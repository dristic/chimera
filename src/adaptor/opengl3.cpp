
#include "src/adaptor/opengl3.h"

#include <stddef.h>
#include <cstdlib>
#include <sys/stat.h>
#include <algorithm>

#include "lodepng/lodepng.h"
extern "C" {
    #include "libnsgif.h"
}

namespace Nova {

std::atomic<bool> gQuitting;

GLuint gUniformLocationProjMatrix;
GLuint gAttribLocationPosition;
GLuint gAttribLocationUV;
GLuint gAttribLocationColor;
GLuint gUniformLocationTex;

GLuint gVboHandle, gElementsHandle, gVaoHandle;

// std::mutex mVideoMutex{};
// std::vector<VideoRef*> OpenGL3Bridge::mVideoRefs = {};

// void OpenGL3Bridge::processVideo() {
//     for (;;) {
//         std::lock_guard<std::mutex> guard(mVideoMutex);

//         for (auto& video : mVideoRefs) {
//             if (video != nullptr) {
//                 video->cap->read(video->frame);

//                 if (video->frame.empty()) {
//                     video->cap->set(CV_CAP_PROP_POS_AVI_RATIO, 0);
//                 }
//             }
//         }

//         std::this_thread::sleep_for(std::chrono::milliseconds(30));

//         if (gQuitting) {
//             printf("Video decoding thread breaking...\n");
//             break;
//         }
//     }
// }

// std::unique_ptr<VideoRef> OpenGL3Bridge::loadVideo(std::string videoPath) {
//     cv::Mat frame;
//     auto cap = std::make_unique<cv::VideoCapture>();
    
//     cap->open(videoPath);
//     if (!cap->isOpened()) {
//         printf("Unable to open video!\n");
//         return nullptr;
//     }

//     cap->read(frame);
//     if (frame.empty()) {
//         printf("Unable to read frame!\n");
//         return nullptr;
//     }

//     GLint lastTextureBinding;
//     glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureBinding);
//     GLint lastTexture;
//     glGetIntegerv(GL_ACTIVE_TEXTURE, &lastTexture);

//     // Create one OpenGL texture
//     GLuint videoTexID;
//     glGenTextures(1, &videoTexID);

//     glBindTexture(GL_TEXTURE_2D, videoTexID);

//     // Give the image to OpenGL
//     cv::Size frameSize = frame.size();
//     glTexImage2D(
//         GL_TEXTURE_2D,
//         0,
//         GL_RGBA,
//         frameSize.width,
//         frameSize.height,
//         0,
//         GL_BGR,
//         GL_UNSIGNED_BYTE,
//         frame.data);

//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

//     glActiveTexture(lastTexture);
//     glBindTexture(GL_TEXTURE_2D, lastTextureBinding);

//     auto v = std::make_unique<VideoRef>(frameSize.width, frameSize.height, videoTexID, std::move(cap));

//     mVideoRefs.push_back(v.get());

//     return v;
// }

OpenGL3Bridge::OpenGL3Bridge()
    : mTextureCache{}
    // , mVideoThread{&OpenGL3Bridge::processVideo}
{
#ifdef _WIN32
    glewInit();
#endif

    mTextureCache.reserve(200);

    initialize();
}

OpenGL3Bridge::~OpenGL3Bridge() {
    gQuitting = true;
    // OpenGL3Bridge::mVideoThread.join();
}

void OpenGL3Bridge::initialize() {
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    createShaders();

    glGenBuffers(1, &gVboHandle);
    glGenBuffers(1, &gElementsHandle);

    glGenVertexArrays(1, &gVaoHandle);
    glBindVertexArray(gVaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, gVboHandle);
    glEnableVertexAttribArray(gAttribLocationPosition);
    glEnableVertexAttribArray(gAttribLocationUV);
    glEnableVertexAttribArray(gAttribLocationColor);

    glVertexAttribPointer(
        gAttribLocationPosition,  // location
        2,  // # of elements
        GL_FLOAT,  // type of each element
        GL_FALSE,  // normalize between (-1, 1) or (0, 1)
        sizeof(DrawVert),  // offset between elements
        reinterpret_cast<GLvoid*>(offsetof(DrawVert, pos)));  // offset
    glVertexAttribPointer(
        gAttribLocationUV,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(DrawVert),
        reinterpret_cast<GLvoid*>(offsetof(DrawVert, uv)));
    glVertexAttribPointer(
        gAttribLocationColor,
        4,
        GL_FLOAT,
        GL_TRUE,
        sizeof(DrawVert),
        reinterpret_cast<GLvoid*>(offsetof(DrawVert, color)));
}

void OpenGL3Bridge::createShaders() {
    const GLchar* vertexShaderSource =
        "#version 330\n"
        "uniform mat4 projectionMatrix;\n"
        "layout(location = 0) in vec2 position;\n"
        "layout(location = 1) in vec2 UV;\n"
        "layout(location = 2) in vec4 color;\n"
        "out vec4 fragColor;\n"
        "out vec2 fragUV;\n"
        "void main() {\n"
            "fragColor = color;\n"
            "fragUV = UV;\n"
            "gl_Position = projectionMatrix * vec4(position.xy, 0, 1);\n"
        "}\n";

    const GLchar* fragmentShaderSource =
        "#version 330\n"
        "in vec4 fragColor;\n"
        "in vec2 fragUV;\n"
        "out vec4 outColor;\n"
        "uniform sampler2D image;\n"
        "void main() {\n"
            "outColor = fragColor * texture(image, fragUV);\n"
        "}\n";

    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, NULL);
    glCompileShader(vertexShaderId);
    glCompileShader(fragmentShaderId);

    GLint result = GL_FALSE;
    int infoLogLength;

    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> VertexShaderErrorMessage(infoLogLength);

    if (infoLogLength > 0) {
        glGetShaderInfoLog(
            vertexShaderId, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
    }

    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> FragmentShaderErrorMessage(infoLogLength);

    if (infoLogLength > 0) {
        glGetShaderInfoLog(
           fragmentShaderId, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramId = glCreateProgram();
    glAttachShader(ProgramId, vertexShaderId);
    glAttachShader(ProgramId, fragmentShaderId);
    glLinkProgram(ProgramId);

    // Check the program
    glGetProgramiv(ProgramId, GL_LINK_STATUS, &result);
    glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> ProgramErrorMessage(
        std::max(infoLogLength, 1));
    glGetProgramInfoLog(
        ProgramId, infoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    glUseProgram(ProgramId);

    gUniformLocationProjMatrix = glGetUniformLocation(
        ProgramId, "projectionMatrix");
    gAttribLocationPosition = glGetAttribLocation(ProgramId, "position");
    gAttribLocationUV = glGetAttribLocation(ProgramId, "UV");
    gAttribLocationColor = glGetAttribLocation(ProgramId, "color");
    gUniformLocationTex = glGetUniformLocation(ProgramId, "image");

    // Create a default blank white texture
    glActiveTexture(GL_TEXTURE0);
    GLubyte texData[] = { 255, 255, 255, 255 };
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
}

unsigned char* load_file(std::string path, size_t *data_size)
{
    FILE *fd;
    struct stat sb;
    unsigned char *buffer;
    size_t size;
    size_t n;

    fd = fopen(path.c_str(), "rb");
    if (!fd) {
        perror(path.c_str());
        exit(EXIT_FAILURE);
    }

    if (stat(path.c_str(), &sb)) {
        perror(path.c_str());
        exit(EXIT_FAILURE);
    }
    size = sb.st_size;

    buffer = (unsigned char *)malloc(size);
    if (!buffer) {
        fprintf(stderr, "Unable to allocate %lld bytes\n",
                (long long) size);
        exit(EXIT_FAILURE);
    }

    n = fread(buffer, 1, size, fd);
    if (n != size) {
        perror(path.c_str());
        exit(EXIT_FAILURE);
    }

    fclose(fd);

    *data_size = size;
    return buffer;
}


void *bitmap_create(int width, int height)
{
    return calloc(width * height, 4);
}


void bitmap_set_opaque(void *bitmap, bool opaque)
{
    (void) opaque;  /* unused */
    (void) bitmap;  /* unused */
}


bool bitmap_test_opaque(void *bitmap)
{
    (void) bitmap;  /* unused */
    return false;
}


unsigned char *bitmap_get_buffer(void *bitmap)
{
    return static_cast<unsigned char *>(bitmap);
}


void bitmap_destroy(void *bitmap)
{
    free(bitmap);
}


void bitmap_modified(void *bitmap)
{
    (void) bitmap;  /* unused */
    return;
}

std::unique_ptr<GifRef> OpenGL3Bridge::loadGIF(std::string imagePath) {
    gif_bitmap_callback_vt bitmap_callbacks = {
        bitmap_create,
        bitmap_destroy,
        bitmap_get_buffer,
        bitmap_set_opaque,
        bitmap_test_opaque,
        bitmap_modified
    };
    gif_animation gif;
    size_t size;
    gif_result code;
    unsigned int i;

    /* create our gif animation */
    gif_create(&gif, &bitmap_callbacks);

    /* load file into memory */
    unsigned char *data = load_file(imagePath, &size);

    /* begin decoding */
    do {
        code = gif_initialise(&gif, size, data);
        if (code != GIF_OK && code != GIF_WORKING) {
            exit(1);
        }
    } while (code != GIF_OK);

    printf("P3\n");
    printf("# width                %u \n", gif.width);
    printf("# height               %u \n", gif.height);
    printf("# frame_count          %u \n", gif.frame_count);
    printf("# frame_count_partial  %u \n", gif.frame_count_partial);
    printf("# loop_count           %u \n", gif.loop_count);
    printf("%u %u 256\n", gif.width, gif.height * gif.frame_count);

    /* decode the frames */
    GLuint* firstTextureId;
    for (i = 0; i != gif.frame_count; i++) {
        unsigned int row, col;
        unsigned char *image;

        code = gif_decode_frame(&gif, i);
        if (code != GIF_OK)
            printf("gif_decode_frame %i\n", code);

        image = (unsigned char *) gif.frame_image;
        for (row = 0; row != gif.height; row++) {
            for (col = 0; col != gif.width; col++) {
                size_t z = (row * gif.width + col) * 4;
            }
        }

        GLint lastTextureBinding;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureBinding);
        GLint lastTexture;
        glGetIntegerv(GL_ACTIVE_TEXTURE, &lastTexture);

        // Create one OpenGL texture
        GLuint textureID;
        glGenTextures(1, &textureID);

        glBindTexture(GL_TEXTURE_2D, textureID);

        // Give the image to OpenGL
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            gif.width,
            gif.height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            image);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glActiveTexture(lastTexture);
        glBindTexture(GL_TEXTURE_2D, lastTextureBinding);

        mTextureCache.push_back(textureID);

        if (i == 0) firstTextureId = &mTextureCache.back();
    }

    /* clean up */
    gif_finalise(&gif);
    free(data);
    
    return std::make_unique<GifRef>(gif.width, gif.height, firstTextureId);
}

unsigned int OpenGL3Bridge::loadTexture(
    unsigned int width,
    unsigned int height,
    unsigned char* buffer)
{
    glActiveTexture(GL_TEXTURE0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Generate texture
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        buffer);

    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureId;
}

std::unique_ptr<ImageRef> OpenGL3Bridge::loadImage(std::string imagePath, int& width, int& height) {
    if (imagePath.compare(imagePath.size() - 3, 3, "gif") == 0) {
        return loadGIF(imagePath);
    }
    //  else if (imagePath.compare(imagePath.size() - 3, 3, "mov") == 0) {
    //     return loadVideo(imagePath);
    // } else if (imagePath.compare(imagePath.size() - 3, 3, "mp4") == 0) {
    //     return loadVideo(imagePath);
    // }

    std::vector<unsigned char> image;  // The raw pixels
    unsigned w, h;
    unsigned error = lodepng::decode(image, w, h, imagePath.c_str());

    // If there is an error, display it
    if (error) {
        printf("Decoder error %s\n", lodepng_error_text(error));
    }

    width = static_cast<int>(w);
    height = static_cast<int>(h);

    GLint lastTextureBinding;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureBinding);
    GLint lastTexture;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &lastTexture);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glActiveTexture(lastTexture);
    glBindTexture(GL_TEXTURE_2D, lastTextureBinding);

    return std::make_unique<ImageRef>(width, height, textureID);
}

void OpenGL3Bridge::renderCallback(DrawData* data) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glActiveTexture(GL_TEXTURE0);

    glViewport(0, 0, data->screenWidth, data->screenHeight);

    float scale = data->screenWidth / data->width;

    const float ortho_projection[4][4] = {
          /*x*/        /*y*/           /*z*/  /*w*/
        { 2.0f/data->width, 0.0f,           0.0f,  0.0f },
        { 0.0f,        2.0f/-data->height,  0.0f,  0.0f },
        { 0.0f,        0.0f,          -1.0f,  0.0f },
        {-1.0f,        1.0f,           0.0f,  1.0f }};

    glUniform1i(gUniformLocationTex, 0);
    glUniformMatrix4fv(
        gUniformLocationProjMatrix, 1, GL_FALSE, &ortho_projection[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, gVboHandle);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(DrawVert) * data->vertices.size(),
        &data->vertices[0], GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gElementsHandle);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(unsigned int) * data->indicies.size(),
        &data->indicies[0], GL_STREAM_DRAW);

    size_t offset = 0;
    for (auto &command : data->commands) {
        GLuint textureId = static_cast<GLuint>(command.textureId);

        glBindTexture(GL_TEXTURE_2D, textureId);

        int sy = data->height - (command.scissor.y + command.scissor.height);

        glScissor(
            command.scissor.x * scale,
            sy * scale,
            command.scissor.width * scale,
            command.scissor.height * scale);

        glDrawElements(
            GL_TRIANGLES,
            command.elements,
            GL_UNSIGNED_INT,
            reinterpret_cast<void*>(offset));

        offset += sizeof(unsigned int) * command.elements;
    }
}

}  // namespace Nova
