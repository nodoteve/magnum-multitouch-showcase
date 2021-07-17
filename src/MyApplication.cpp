#include <Magnum/Math/Color.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/ImGuiIntegration/Context.hpp>

#ifndef CORRADE_TARGET_ANDROID
#include <Magnum/Platform/Sdl2Application.h>
#else
#include <Magnum/Platform/AndroidApplication.h>
#endif

using namespace Magnum;

#include <map>
#include <utility>

class MyApplication : public Platform::Application
{
public:
    explicit MyApplication(const Arguments &arguments);

    void drawEvent() override;

    void mousePressEvent(MouseEvent &event) override;
    void mouseReleaseEvent(MouseEvent &event) override;
    void mouseMoveEvent(MouseMoveEvent &event) override;

private:
    ImGuiIntegration::Context imgui{NoCreate};
    std::map<int,std::pair<int,int>> data;
};

MyApplication::MyApplication(const Arguments &arguments) : Platform::Application{arguments}
{
    imgui = ImGuiIntegration::Context(Vector2{windowSize()} / dpiScaling(),
                                      windowSize(), framebufferSize());

    /* Set up proper blending to be used by ImGui. There's a great chance
       you'll need this exact behavior for the rest of your scene. If not, set
       this only for the drawFrame() call. */
    GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add,
                                   GL::Renderer::BlendEquation::Add);
    GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha,
                                   GL::Renderer::BlendFunction::OneMinusSourceAlpha);

#if !defined(MAGNUM_TARGET_WEBGL) && !defined(CORRADE_TARGET_ANDROID)
    /* Have some sane speed, please */
    setMinimalLoopPeriod(16);
#endif
}

void MyApplication::drawEvent()
{
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

    imgui.newFrame();

    /* 1. Show a simple window.
       Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appear in
       a window called "Debug" automatically */
    {
        ImGui::Begin("Hello, world!");
        ImGui::SetWindowPos(ImVec2(650, 350));
        ImGui::Text("Hello, world!");
        ImGui::End();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0/Double(ImGui::GetIO().Framerate), Double(ImGui::GetIO().Framerate));


        for(auto&e:data){
        auto& a=e.first;
        auto& b=e.second;
        auto s=std::to_string(a).data();
        ImGui::Begin(s);
        ImGui::SetWindowPos(ImVec2(b.first,b.second));
        ImGui::SetWindowSize(ImVec2(100,100));
        ImGui::Text(s);
        ImGui::End();
        }
    }

    /* Update application cursor */
    // imgui.updateApplicationCursor(*this);

    /* Set appropriate states. If you only draw ImGui, it is sufficient to
       just enable blending and scissor test in the constructor. */
    GL::Renderer::enable(GL::Renderer::Feature::Blending);
    GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
    GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::disable(GL::Renderer::Feature::DepthTest);

    imgui.drawFrame();

    /* Reset state. Only needed if you want to draw something else with
       different state after. */
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::disable(GL::Renderer::Feature::ScissorTest);
    GL::Renderer::disable(GL::Renderer::Feature::Blending);

    swapBuffers();
    redraw();
}

void MyApplication::mousePressEvent(MouseEvent &event)
{
    #ifdef CORRADE_TARGET_ANDROID
    data[event.pointerId()]={event.position().x(),event.position().y()};
    Debug{}<<"a"<<event.pointerId()<<" "<<event.pointerIndex();

    #else
    data[1]={event.position().x(),event.position().y()};
    #endif


    if (imgui.handleMousePressEvent(event))
        return;

}

void MyApplication::mouseReleaseEvent(MouseEvent &event)
{
    #ifdef CORRADE_TARGET_ANDROID
    data.erase(event.pointerId());
    Debug{}<<"r"<<event.pointerId()<<" "<<event.pointerIndex();

    #else
    data[1]={0,0};
    #endif

    if (imgui.handleMouseReleaseEvent(event))
        return;

}

void MyApplication::mouseMoveEvent(MouseMoveEvent &event)
{
    #ifdef CORRADE_TARGET_ANDROID
    //    data[event.pointerId()]={event.position().x(),event.position().y()};
    // Debug{}<<"a"<<event.pointerId()<<" "<<event.pointerIndex();
    #else
    data[1]={event.position().x(),event.position().y()};
    #endif

    if (imgui.handleMouseMoveEvent(event))
        return;

}

MAGNUM_APPLICATION_MAIN(MyApplication)
