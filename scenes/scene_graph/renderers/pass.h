#include "../../../ogl/render_targets/render_target.h"
#include "../../camera/camera.h"
#include "../nodes/node.h"

struct RenderPass {
    std::string name;
    bool depth_write_enabled;
    bool depth_test_enabled;
    OpenGL::RenderTarget render_target;
    Camera* camera;
    SceneGraph::Node* root_node;
};





struct RenderChain {
    
    std::vector<RenderPass> passes;
    
    
    
};





