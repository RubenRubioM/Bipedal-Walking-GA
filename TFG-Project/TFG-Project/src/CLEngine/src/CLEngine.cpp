/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 #include "CLEngine.h"

using namespace std;
using namespace CLE;

/**
 * Funcion callback de errores de GLFW.
 * @param error - ID del error.
 * @param descripcion - Detalles sobre el error.
 */
static void error(int error, const char* description) {
    cerr << "Error (" << error << "): " << description << endl;
}

/**
 * Constructor que iniciara todas las configuraciones iniciales de OpenGL y la ventana.
 * @param w - Anchura en pixeles de la ventana.
 * @param h - Altura en pixeles de la ventana.
 * @param title - Titulo de la ventana.
 */
CLEngine::CLEngine (const unsigned int w, const unsigned int h, const string& title) : width(w), height(h) {
    CreateGlfwWindow(w, h, title);
    glewInit();
    ImGuiInit();
    const std::string f = "fonts/Plump.ttf";

    smgr = make_unique<CLNode>();

    //Vamos a leer todos los shaders al iniciar
    CLResourceManager* resourceManager = CLResourceManager::GetResourceManager();

    shaders.emplace_back(resourceManager->GetResourceShader("src/CLEngine/src/Shaders/shadowMappingShader.vert", "src/CLEngine/src/Shaders/shadowMappingShader.frag")->GetProgramID());
    shaders.emplace_back(resourceManager->GetResourceShader("src/CLEngine/src/Shaders/cartoonShader.vert", "src/CLEngine/src/Shaders/cartoonShader.frag")->GetProgramID());
    shaders.emplace_back(resourceManager->GetResourceShader("src/CLEngine/src/Shaders/lightMapping.vert", "src/CLEngine/src/Shaders/lightMapping.frag")->GetProgramID());
    shaders.emplace_back(resourceManager->GetResourceShader("src/CLEngine/src/Shaders/basicShader.vert", "src/CLEngine/src/Shaders/basicShader.frag")->GetProgramID());

}

/**
 * Destruye la ventana de GLFW y libera la informacion.
 */
CLEngine::~CLEngine() {
    TerminateImGui();
    glfwDestroyWindow(window);
    glfwTerminate();
    //cout << ">>>>> GLFW OFF" << endl;
}

/**
 * Inicia, crea y configura  la ventana OpenGL con GLFW.
 * @param w - Anchura en pixeles de la ventana.
 * @param h - Altura en pixeles de la ventana.
 * @param title - Titulo de la ventana.
 */
void CLEngine::CreateGlfwWindow (const unsigned int w, const unsigned int h, const string& title) {

    glfwSetErrorCallback(error);

    if (!glfwInit()) {
        //cout << "- No se ha podido inicializar GLFW" << endl;
        exit(EXIT_FAILURE);
    }
    //cout << ">>>>> GLFW ON" << endl;

    // Minima version de OpenGL, si la maquina no puede correr minimo a la indicada, falla la creacion de la ventana.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( w, h, title.c_str(), nullptr, nullptr );
    if (!window) {
        //cout << "    > La ventana no se ha podido crear" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    //cout << "    > Ventana creada" << endl;


    glfwMakeContextCurrent(window);

    // Por defecto esta a 0, pero parece que eso despercicia ciclos de CPU y GPU. Se recomienda ponerlo a 1.
    glfwSwapInterval(1);

    // Activa el buffer de profundidad o ZBuffer, para que se diferencie que pixel se debe pintar.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    
}

/**
 * Activa o desactiva el test de profundidad
 */
void CLEngine::SetEnableDepthTest(bool b) {
    if (b)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

CLNode* CLEngine::GetSceneManager(){
    return smgr.get();
}

CLResourceManager* CLEngine::GetResourceManager(){
    return CLResourceManager::GetResourceManager();
}

/**
 * Actualizacion de CLEngine.
 */
bool CLEngine::Run() {
    return !(glfwWindowShouldClose(window));
}

void CLEngine::PollEvents(){
    glfwPollEvents();
}

/**
 * Limpia la pantalla pintandola de un color.
 */
void CLEngine::BeginScene(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void CLEngine::DrawDepthMap(const glm::mat4& lightSpaceMatrix){
    //auto light = GetNodeByID(GetShadowMapping()->GetID());

    // 1. Se renderiza con el shadowMap
    glCullFace(GL_FRONT);
    RenderDepthMap(*GetShadowMapping(), GetDepthShader(), lightSpaceMatrix);
    glCullFace(GL_BACK);

    // 2. then render scene as normal with shadow mapping (using depth map)
    UpdateViewport();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}



void CLEngine::DrawObjects(){
    glm::mat4 lightSpaceMatrix;
    if(shadowMapping && shadowsActivate){
        auto light = GetNodeByID(GetShadowMapping()->GetID());
        glm::mat4 lightProjection, lightView;
        float near_plane = 1.0f, far_plane = 5000.0f;
        lightProjection = glm::ortho(-float(1500), float(1500),  -float(1500), float(1500), near_plane, far_plane);
        lightView = glm::lookAt(light->GetGlobalTranslation(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        DrawDepthMap(lightSpaceMatrix);
    }

    DrawSkybox();
    CalculateViewProjMatrix(lightSpaceMatrix);
    CalculateLights();
    glm::mat4 VPmatrix = projection*view;
    smgr->DFSTree(glm::mat4(1.0f),GetActiveCamera(), VPmatrix);
}

/**
 * Renderiza las cosas de ImGui y cambia el buffer de la ventana. 
 */
void CLEngine::EndScene(){
    
    glfwSwapBuffers(window);

}

/**
 *
 */
void CLEngine::SetTitle(string &t) {
    glfwSetWindowTitle(window, t.c_str());
}

void CLEngine::SetTitle(string &&t) {
    glfwSetWindowTitle(window, t.c_str());
}

/**
 * Devuelve el tiempo de glfw
 */
double CLEngine::GetTime(){
    return glfwGetTime();
}

void CLEngine::DisableCursor(){
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
/**
 * Mira si se han actualizado los valores de anchura y altura de la ventana y actualiza el viewport.
 */
void CLEngine::UpdateViewport(){
    glfwGetFramebufferSize(window, &width, &height);
    // glfwSetWindowSize(window, width, height);
    glViewport(0, 0, width, height);
    // cout << "width["<<width<<"] height["<<height<<"]" << endl;
}


void CLEngine::CloseWindow(){
    glfwSetWindowShouldClose(window, true);

}

void CLEngine::Clear(){
    if (smgr) {
        for (const auto& c : smgr->GetChilds()) {
            smgr->RemoveChild(c.get());
        }
        RemoveLightsAndCameras();
    }
}


// -----------------------------------------------------------
//  IMGUI
// -----------------------------------------------------------

void CLEngine::ImGuiInit(){
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
    ImGui::StyleColorsDark();
}

void CLEngine::RenderImgui() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void CLEngine::TerminateImGui(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

/// --------------------------------
///  SOMBRAS
/// --------------------------------

// Renderiza el depth map
void CLEngine::RenderDepthMap(CLShadowMapping& shadowMap, CLResourceShader* depthShader, const glm::mat4& lightSpaceMatrix){
    glUseProgram(depthShader->GetProgramID());

    // renderizar escena para el depth cubemap
    glViewport(0, 0, shadowMap.SHADOW_WIDTH,  shadowMap.SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER,  shadowMap.depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    smgr->DFSTree(glm::mat4(1.0f), GetActiveCamera(), depthShader->GetProgramID(), lightSpaceMatrix);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



//Methods
void CLEngine::AddShader(const string vertex, const string fragment){
    shaders.emplace_back(resourceManager->GetResourceShader(vertex, fragment)->GetProgramID());
    
}

void CLEngine::AddShader(const string vertex, const string fragment, const string geometry){
    shaders.emplace_back(resourceManager->GetResourceShader(vertex, fragment,geometry)->GetProgramID());
    
}

CLNode* CLEngine::AddGroup(CLNode* parent,unsigned int id){
    shared_ptr<CLNode> node = make_shared<CLNode>();
    parent->AddChild(node);

    return node.get();
}

CLNode* CLEngine::AddMesh(CLNode* parent,unsigned int id){
    
    shared_ptr<CLEntity> e = make_shared<CLMesh>(id);
    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);


    return node.get();
}

CLNode* CLEngine::AddMesh(CLNode* parent,unsigned int id,string mesh){
    auto node = AddMesh(parent, id);
    auto resourceMesh = CLResourceManager::GetResourceManager()->GetResourceMesh(mesh);
    static_cast<CLMesh*>(node->GetEntity())->SetMesh(resourceMesh);

    return node;
}

CLNode* CLEngine::AddPointLight(CLNode* parent,unsigned int id){
    
    shared_ptr<CLEntity> e = make_shared<CLPointLight>(id);
    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);
    pointLights.push_back(node.get());
    return node.get();
}

CLNode* CLEngine::AddPointLight(CLNode* parent,unsigned int id,glm::vec3 intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic){
    auto node = AddPointLight(parent, id);
    static_cast<CLPointLight*>(node->GetEntity())->SetLightAttributes(intensity,ambient,diffuse,specular,constant,linear,quadratic);
    return node;
}

CLNode* CLEngine::AddCamera(CLNode* parent,unsigned int id){
   
    shared_ptr<CLEntity> e = make_shared<CLCamera>(id);
    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);
    cameras.push_back(node.get());

    return node.get(); 
}

CLNode* CLEngine::AddParticleSystem(CLNode* parent,unsigned int id, unsigned int nParticles,glm::vec3 velocity,vector<string> textures,int width,int height,int spawnDelay,int particlesToSpawn,int lifeSpan, std::uint_fast8_t flags){
    if(particleSystemShader == 0){
        auto rm = CLResourceManager::GetResourceManager();
        auto resourceShader = rm->GetResourceShader("src/CLEngine/src/Shaders/particleSystem.vert", "src/CLEngine/src/Shaders/particleSystem.frag","src/CLEngine/src/Shaders/particleSystem.geom");
        particleSystemShader = resourceShader->GetProgramID();
        shaders.push_back(particleSystemShader);
    }


    shared_ptr<CLEntity> e = make_shared<CLParticleSystem>(id,nParticles,velocity,textures,width,height,spawnDelay,particlesToSpawn,lifeSpan,flags);

    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);
    node->SetShaderProgramID(particleSystemShader);

    //Configuraciones especificas de un particlesystem
    if(auto particleSystem = dynamic_cast<CLParticleSystem*>(e.get())){
        particleSystem->SetCLNode(node.get());
    }
    return node.get();
}

CLNode* CLEngine::AddParticleSystem(CLNode* parent,unsigned int id,unsigned int nParticles,glm::vec3 velocity,vector<string> textures,int width,int height,int spawnDelay,int particlesToSpawn,int lifeSpan,glm::vec3 offset, glm::vec3 orientation, std::uint_fast8_t flags){
    if(particleSystemShader == 0){
        auto rm = CLResourceManager::GetResourceManager();
        auto resourceShader = rm->GetResourceShader("src/CLEngine/src/Shaders/particleSystem.vert", "src/CLEngine/src/Shaders/particleSystem.frag","src/CLEngine/src/Shaders/particleSystem.geom");
        particleSystemShader = resourceShader->GetProgramID();
        shaders.push_back(particleSystemShader);

    }


    shared_ptr<CLEntity> e = make_shared<CLParticleSystem>(id,nParticles,velocity,textures,width,height,spawnDelay,particlesToSpawn,lifeSpan,offset,orientation,flags);

    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);
    node->SetShaderProgramID(particleSystemShader);

    //Configuraciones especificas de un particlesystem
    if(auto particleSystem = dynamic_cast<CLParticleSystem*>(e.get())){
        particleSystem->SetCLNode(node.get());
    }
    return node.get();
}

CLNode* CLEngine::AddParticleSystem(CLNode* parent,unsigned int id,unsigned int nParticles,glm::vec3 velocity,vector<string> textures,int width,int height,int spawnDelay,int particlesToSpawn,int lifeSpan,float radious, std::uint_fast8_t flags){
    if(particleSystemShader == 0){
        auto rm = CLResourceManager::GetResourceManager();
        auto resourceShader = rm->GetResourceShader("src/CLEngine/src/Shaders/particleSystem.vert", "src/CLEngine/src/Shaders/particleSystem.frag","src/CLEngine/src/Shaders/particleSystem.geom");
        particleSystemShader = resourceShader->GetProgramID();
        shaders.push_back(particleSystemShader);

    }

    

    shared_ptr<CLEntity> e = make_shared<CLParticleSystem>(id,nParticles,velocity,textures,width,height,spawnDelay,particlesToSpawn,lifeSpan,radious,flags);

    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);
    node->SetShaderProgramID(particleSystemShader);

    //Configuraciones especificas de un particlesystem
    if(auto particleSystem = dynamic_cast<CLParticleSystem*>(e.get())){
        particleSystem->SetCLNode(node.get());
    }

    return node.get();
}

CLNode* CLEngine::AddParticleSystem(CLNode* parent,unsigned int id,unsigned int nParticles,glm::vec3 velocity,vector<string> textures,int width,int height,int spawnDelay,int particlesToSpawn,int lifeSpan,float radious,glm::vec3 orientation, std::uint_fast8_t flags){
    if(particleSystemShader == 0){
        auto rm = CLResourceManager::GetResourceManager();
        auto resourceShader = rm->GetResourceShader("src/CLEngine/src/Shaders/particleSystem.vert", "src/CLEngine/src/Shaders/particleSystem.frag","src/CLEngine/src/Shaders/particleSystem.geom");
        particleSystemShader = resourceShader->GetProgramID();
        shaders.push_back(particleSystemShader);

    }

    

    shared_ptr<CLEntity> e = make_shared<CLParticleSystem>(id,nParticles,velocity,textures,width,height,spawnDelay,particlesToSpawn,lifeSpan,radious,orientation,flags);

    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);
    node->SetShaderProgramID(particleSystemShader);

    //Configuraciones especificas de un particlesystem
    if(auto particleSystem = dynamic_cast<CLParticleSystem*>(e.get())){
        particleSystem->SetCLNode(node.get());
    }
    return node.get();
}


void CLEngine::AddSkybox(string right, string left, string top, string bottom, string front, string back){
    if(!skyboxShader){
        auto rm = CLResourceManager::GetResourceManager();
        auto resourceShader = rm->GetResourceShader("src/CLEngine/src/Shaders/skybox.vert", "src/CLEngine/src/Shaders/skybox.frag");
        skyboxShader = resourceShader->GetProgramID();
        shaders.push_back(skyboxShader);

    }
    skybox = make_unique<CLSkybox>(right, left, top, bottom, front, back);
    
}

void CLEngine::AddShadowMapping(GLuint lightId){
    if(!simpleDepthShader){
        auto rm = CLResourceManager::GetResourceManager();
        depthShadder = rm->GetResourceShader("src/CLEngine/src/Shaders/simpleDepthShader.vert", "src/CLEngine/src/Shaders/simpleDepthShader.frag");
        simpleDepthShader = depthShadder->GetProgramID();
        shaders.push_back(simpleDepthShader);
        
    }
    shadowMapping = make_unique<CLShadowMapping>(lightId);
}

CLNode* CLEngine::AddBillBoard(CLNode* parent,unsigned int id,string& file, bool vertically, float width_, float height_){
    auto rm = CLResourceManager::GetResourceManager();
    if(!billboardShader){
        auto resourceShader = rm->GetResourceShader("src/CLEngine/src/Shaders/billboard.vert", "src/CLEngine/src/Shaders/billboard.frag", "src/CLEngine/src/Shaders/billboard.geom");
        billboardShader = resourceShader->GetProgramID();
        shaders.push_back(billboardShader);
        
    }

    

    CLResourceTexture* texture = rm->GetResourceTexture(file, vertically);
    auto entity = make_shared<CLBillboard>(id,texture,width_,height_);
    auto node = make_shared<CLNode>(entity);

    parent->AddChild(node);
    node->SetShaderProgramID(billboardShader);


    return node.get();
}

bool CLEngine::RemoveChild(CLNode* child){
    return smgr->RemoveChild(child);
}

bool CLEngine::HasChild(CLNode* child){
    return smgr->HasChild(child);
}

CLNode* CLEngine::GetNodeByID(unsigned int id){
    CLNode* node = nullptr;
    node = GetNodeByIDAux(id, node, smgr.get());
    return node;
}


CLNode* CLEngine::GetNodeByIDAux(unsigned int id, CLNode* node, CLNode* root){
 
    if(node!=nullptr) return node; //Caso base, ha encontrado ya al nodo que busca
    if(root->GetChilds().size()>0){
        //Tiene hijos
        for(auto& nodo : root->GetChilds()){
            if(nodo->GetEntity() && nodo->GetEntity()->GetID() == id){
                node = nodo.get();
                return node;
            }else{
                node = GetNodeByIDAux(id, node, nodo.get());

            }
        }

    }

    return node;
}


// devolver datos de la camara
float CLEngine::GetFovActualCamera(){
    return static_cast<CLCamera*>(GetActiveCamera())->GetCameraFov();
}
glm::vec3 CLEngine::GetTargetActualCamera(){
    return static_cast<CLCamera*>(GetActiveCamera())->GetCameraTarget();
}
glm::vec3 CLEngine::GetPositionActualCamera(){
    return GetActiveCameraNode()->GetGlobalTranslation();
}

float CLEngine::GetBoundingSizeById(unsigned int id){
    CLNode* node = GetNodeByID(id);
    return node->CalculateBoundingBox();
}

void CLEngine::SetParticlesVisibility(bool mode){
    smgr->SetParticlesActivated(mode);
}


// Comprueba si el cubo del octree se ve en la camara del jugador
bool CLEngine::OctreeIncamera(float size, const glm::vec3& pos){
    glm::vec3 pos2 = glm::vec3(pos.x, pos.y, -pos.z);
    glm::vec3 pointBox[]{
        {pos2 - glm::vec3(size, size, size)},
        {pos2 - glm::vec3(size, size, -size)},
        {pos2 - glm::vec3(size, -size, size)},
        {pos2 - glm::vec3(size, -size, -size)},
        {pos2 - glm::vec3(-size, size, size)},
        {pos2 - glm::vec3(-size, size, -size)},
        {pos2 - glm::vec3(-size, -size, size)},
        {pos2 - glm::vec3(-size, -size, -size)}
    };

    CLCamera* camera = GetActiveCamera();
    CLNode* nodeCam = GetNodeByID(camera->GetID());

    float vectorToTargetX = camera->GetCameraTarget().x - nodeCam->GetGlobalTranslation().x;
    float vectorToTargetZ = camera->GetCameraTarget().z - nodeCam->GetGlobalTranslation().z;
    float valueCentralDegree = glm::degrees( atan2(vectorToTargetZ, vectorToTargetX) );
    if (valueCentralDegree < 0) valueCentralDegree += 360;

    float minVision = valueCentralDegree - camera->GetCameraFov();
    if (minVision < 0) minVision += 360;

    float maxVision = valueCentralDegree + camera->GetCameraFov();
    if (maxVision >= 360) maxVision -= 360;

    // comprobamos si algun punto del cubo se encuentra dentro de la camara
    for(unsigned int i=0; i<8; i++){
        float vectorToBoxX = pointBox[i].x - nodeCam->GetGlobalTranslation().x;
        float vectorToBoxZ = pointBox[i].z - nodeCam->GetGlobalTranslation().z;
        float valueDegreeBox = glm::degrees( atan2(vectorToBoxZ, vectorToBoxX) );
        if (valueDegreeBox < 0) valueDegreeBox += 360;

        if(minVision>maxVision){
            if(minVision<valueDegreeBox || maxVision>valueDegreeBox)
                return true;
        }else{
            if(minVision<valueDegreeBox && maxVision>valueDegreeBox)
                return true;
        }
    }

    // no se ve en caso que que ningun punto este dentro de la camara
    return false;
}



bool CLEngine::DeleteNode(unsigned int id){
    CLNode* node = nullptr;
    node = GetNodeByIDAux(id, node, smgr.get());
    if(!node) return false;
    auto father = node->GetFather();
    father->RemoveChild(node);
    return true;
}

bool CLEngine::DeleteNode(CLNode* node){
    if(!node) return false;
    auto father = node->GetFather();
    father->RemoveChild(node);
    return true;
}


void CLEngine::DrawSkybox(){
    if(skybox.get()){
        glDepthMask(GL_FALSE);
        glUseProgram(skyboxShader);

        glm::mat4 view2 = glm::mat4(glm::mat3(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 1, GL_FALSE, glm::value_ptr(view2));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        skybox->Draw(skyboxShader);
    }
}

CLCamera* CLEngine::GetActiveCamera(){
    for(auto camera : cameras){
        auto entityCamera = static_cast<CLCamera*>(camera->GetEntity());
        if(entityCamera->IsActive()){
            return entityCamera;
        }
    }
    return nullptr;
}

CLNode* CLEngine::GetActiveCameraNode(){
    for(auto camera : cameras){
        auto entityCamera = static_cast<CLCamera*>(camera->GetEntity());
        if(entityCamera->IsActive()){
            return camera;
        }
    }
    return nullptr;
}


const void CLEngine::Draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2) const{
    Draw3DLine(x1,y1,z1,x1,y2,z1,CLColor(255.0,0.0,0.0,255.0));
}

const void CLEngine::Draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2,CLColor color) const{

    if(!debugShader){
        auto rm = CLResourceManager::GetResourceManager();
        auto resourceShader = rm->GetResourceShader("src/CLEngine/src/Shaders/debugShader.vert", "src/CLEngine/src/Shaders/debugShader.frag");
        debugShader = resourceShader->GetProgramID();
    }
    float line[] = {
        x1, y1, z1,
        x2, y2, z2
    };
    
    //glEnable(GL_LINE_SMOOTH);
    glLineWidth(lineWidth);
    //glHint(GL_LINE_SMOOTH_HINT,  GL_NICEST);

    unsigned int VBOLine, VAOLine;
    glGenBuffers(1, &VBOLine);
    glGenVertexArrays(1, &VAOLine);
    glBindVertexArray(VAOLine);
    glBindBuffer(GL_ARRAY_BUFFER, VBOLine);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  3 * sizeof(float), 0);
    glBindVertexArray(0);


    glUseProgram(debugShader);

    glm::vec4 clcolor(color.GetRedNormalized(),color.GetGreenNormalized(),color.GetBlueNormalized(),color.GetAlphaNormalized());
    glUniformMatrix4fv(glGetUniformLocation(debugShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(debugShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform4fv(glGetUniformLocation(debugShader, "clcolor"),1, glm::value_ptr(clcolor));

    glBindVertexArray(VAOLine);
    glDrawArrays(GL_LINE_LOOP, 0,2); 
    glUseProgram(0);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAOLine);
    glDeleteBuffers(1, &VBOLine);

} 

// Mi idea ahora es pasarle todas las luces a todos los shaders que las vayan a usar
void CLEngine::CalculateLights(){
    GLuint i = 0;

    for(auto shader : shaders){
        glUseProgram(shader);
        for(auto pointLight : pointLights){
            auto pointLightEntity = static_cast<CLPointLight*>(pointLight->GetEntity());
            
            string number = to_string(i); 
            //Tenemos que mirar que luz es la del shadowmapping
            if(shadowMapping && pointLight->GetEntity()->GetID() == shadowMapping->GetID()){
                glUniform1i(glGetUniformLocation(shader,"id_luz_shadowMapping"),i);    

            }
            glUniform1i(glGetUniformLocation(shader,"num_Point_Lights"),pointLights.size());    
            glUniform3fv(glGetUniformLocation(shader, ("pointLights[" + number + "].position").c_str()),1,glm::value_ptr(pointLight->GetGlobalTranslation()));
            glUniform3fv(glGetUniformLocation(shader, ("pointLights[" + number + "].ambient").c_str()), 1,glm::value_ptr(pointLightEntity->GetAmbient()));
            glUniform3fv(glGetUniformLocation(shader, ("pointLights[" + number + "].diffuse").c_str()), 1, glm::value_ptr(pointLightEntity->GetDiffuse()));
            glUniform3fv(glGetUniformLocation(shader, ("pointLights[" + number + "].specular").c_str()), 1, glm::value_ptr(pointLightEntity->GetSpecular()));
            glUniform1f(glGetUniformLocation(shader, ("pointLights[" + number + "].constant").c_str()), pointLightEntity->GetConstant());
            glUniform1f(glGetUniformLocation(shader, ("pointLights[" + number + "].linear").c_str()), pointLightEntity->GetLinear());
            glUniform1f(glGetUniformLocation(shader, ("pointLights[" + number + "].quadratic").c_str()), pointLightEntity->GetQuadratic());


            i++;
        }
    }
}

void CLEngine::RemoveLightsAndCameras() {
    cameras.clear();
    pointLights.clear();
    shadowMapping = nullptr;
    skybox = nullptr;
}

void CLEngine::CalculateViewProjMatrix(const glm::mat4& lightSpaceMatrix){
    auto cam = GetActiveCameraNode();
    auto entityCamera = GetActiveCamera();

    projection = entityCamera->CalculateProjectionMatrix();
    view = glm::lookAt(cam->GetGlobalTranslation(),entityCamera->GetCameraTarget(),entityCamera->GetCameraUp());
    glm::mat4 viewProjection = projection*view;
    glm::vec3 camPos = GetActiveCameraNode()->GetGlobalTranslation();

    glm::vec3 directionShadowLight(0.0);
    if(shadowMapping && shadowsActivate){
        auto light = GetNodeByID(GetShadowMapping()->GetID());
        directionShadowLight = glm::normalize(light->GetGlobalTranslation() - glm::vec3(0.0));
    }

    for(auto shader : shaders){
        glUseProgram(shader);

        glUniform3fv(glGetUniformLocation(shader,"viewPos"),1,glm::value_ptr(cam->GetGlobalTranslation()));
        glUniformMatrix4fv(glGetUniformLocation(shader, "VPMatrix"), 1, GL_FALSE, glm::value_ptr(viewProjection));
        glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(glGetUniformLocation(shader, "cameraPosition"), 1, glm::value_ptr(camPos));
        glUniform1i(glGetUniformLocation(shader, "activeShadows"), shadowsActivate); 
        glUniformMatrix4fv(glGetUniformLocation(shader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniform3fv(glGetUniformLocation(shader, "lightShadowDir"), 1, glm::value_ptr(directionShadowLight));
    }
}


void CLEngine::SetOctreeVisibleById(unsigned int id, bool v){
    CLNode* node = GetNodeByID(id);
    node->SetOctreeVisible(v);
}