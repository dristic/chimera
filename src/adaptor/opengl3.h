// Copyright 2016 Dan Ristic

#ifndef EXAMPLES_OPENGL3BRIDGE_H_
#define EXAMPLES_OPENGL3BRIDGE_H_

#ifdef _WIN32
    #include <GL\glew.h>
#endif

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <vector>
#include <unordered_map>
#include <atomic>
#include <thread>

#include "src/context.h"
#include "src/rendering.h"
#include "src/types.h"
#include "src/adaptor/adaptor.h"

namespace Cosmonaut {

class GifRef: public ImageRef {
 public:
    GifRef(int width, int height, GLuint* start)
        : ImageRef(width, height, *start)
        , textureIdStart{start}
        , frame{0}
        { }

    GLuint* textureIdStart;
    int frame;

    void render() {
        frame++;
        if (frame % 2 == 0) {
            textureId++;
        }
        if (frame > 2 * 18) {
            frame = 0;
            textureId = *textureIdStart;
        }
    }
};

// class VideoRef: public ImageRef {
//  public:
//     VideoRef(int width, int height, GLuint texId, std::unique_ptr<cv::VideoCapture> video)
//         : ImageRef(width, height, texId)
//         , frame{}
//         , cap{std::move(video)}
//         { }

//     cv::Mat frame;
//     std::unique_ptr<cv::VideoCapture> cap;

//     void render() {
//         if (!frame.empty()) {
//             glBindTexture(GL_TEXTURE_2D, textureId);

//             cv::Size frameSize = frame.size();
//             glTexImage2D(
//                 GL_TEXTURE_2D,
//                 0,
//                 GL_RGBA,
//                 frameSize.width,
//                 frameSize.height,
//                 0,
//                 GL_BGR,
//                 GL_UNSIGNED_BYTE,
//                 frame.data);
//         }
//     }
// };

class OpenGL3Bridge : public Cosmonaut::Adaptor {
 public:
    OpenGL3Bridge();
    ~OpenGL3Bridge();

    void initialize();
    void createShaders();
    void loadFont(Renderer* renderer);

    static void processVideo();

    std::unique_ptr<GifRef> loadGIF(std::string imagePath);
    // std::unique_ptr<VideoRef> loadVideo(std::string videoPath);
    std::unique_ptr<ImageRef> loadImage(std::string imagePath, int& width, int& height) override;
    unsigned int loadTexture(unsigned int width, unsigned int height, unsigned char* buffer) override;
    void renderCallback(DrawData* data) override;

 private:
    std::vector<GLuint> mTextureCache;
    // std::thread mVideoThread;
    // static std::vector<VideoRef*> mVideoRefs;
};

}  // namespace Cosmonaut

#endif  // EXAMPLES_OPENGL3BRIDGE_H_
