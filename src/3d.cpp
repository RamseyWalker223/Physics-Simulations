#include "3d.h"

cube::cube(){
    body.points = {
        //Points               Texture coords
        -1.0f, -1.0f, 1.0f,    0.0f, 0.0f,
        -1.0f, 1.0f,  1.0f,    0.0f, 1.0f,
         1.0f, 1.0f,  1.0f,    1.0f, 1.0f,
         1.0f, -1.0f, 1.0f,    1.0f, 0.0f,

        -1.0f, -1.0f, -1.0f,    1.0f, 0.0f,
        -1.0f, 1.0f,  -1.0f,    1.0f, 1.0f,
         1.0f, 1.0f,  -1.0f,    0.0f, 1.0f,
         1.0f, -1.0f, -1.0f,    0.0f, 0.0f
    };
        body.indices = {
        0, 2, 1,
        0, 3, 2,

        3, 6, 2,
        3, 7, 6,

        0, 1, 5,
        0, 5, 4,

        7, 5, 6,
        7, 4, 5,

        1, 2, 6,
        1, 6, 5,

        0, 4, 7,
        0, 7, 3
    };
}

void sphere::generate(int resX, int resY, float radius){
    this->radius = radius;
    int x = 0;
    int y = 2;
    float phi, theta, phi_next, theta_next;
    phi = -90.0f;
    theta = 0.0f;
    float sin_phi, cos_phi, sin_theta, cos_theta;
    sin_phi = sin(glm::radians(phi));
    cos_phi = cos(glm::radians(phi));
    sin_theta = sin(glm::radians(theta));
    cos_theta = cos(glm::radians(theta));

    body.points.emplace_back(cos_phi * radius * sin_theta);
    body.points.emplace_back(sin_phi * radius);
    body.points.emplace_back(cos_phi * radius * cos_theta);

    phi = phi + (180.0f/resY);

    int size;

    while(x < resX){
        sin_phi = sin(glm::radians(phi));
        cos_phi = cos(glm::radians(phi));
        sin_theta = sin(glm::radians(theta));
        cos_theta = cos(glm::radians(theta));

        body.points.emplace_back(cos_phi * radius * sin_theta);
        body.points.emplace_back(sin_phi * radius);
        body.points.emplace_back(cos_phi * radius * cos_theta);

        size = body.points.size()/3 - 1;

        body.indices.emplace_back(0);
        body.indices.emplace_back(size);
        body.indices.emplace_back(size - 1);

        theta = theta + 360.0f/resX;
        x++;
    }

    theta = 0.0f;

    //We are calculating and storing the top point over and over again, but whatever. Good enough
    while(y <= resY){
        phi = phi + (180.0f/resY);
        cos_phi = cos(glm::radians(phi));
        sin_phi = sin(glm::radians(phi));
        sin_theta = sin(glm::radians(theta));
        cos_theta = cos(glm::radians(theta));

        body.points.emplace_back(cos_phi*radius*sin_theta);
        body.points.emplace_back(sin_phi*radius);
        body.points.emplace_back(cos_phi*radius*cos_theta);

        while(x < resX){
            theta = theta + 360.0f/resX;
            sin_theta = sin(glm::radians(theta));
            cos_theta = cos(glm::radians(theta));

            body.points.emplace_back(cos_phi*radius*sin_theta);
            body.points.emplace_back(sin_phi*radius);
            body.points.emplace_back(cos_phi*radius*cos_theta);

            size = body.points.size()/3 - 1;

            body.indices.emplace_back(size - (1 + resX));
            body.indices.emplace_back(size - resX);
            body.indices.emplace_back(size);

            body.indices.emplace_back(size - (1 + resX));
            body.indices.emplace_back(size);
            body.indices.emplace_back(size - 1);

            x++;
        }
        //body.indices.emplace_back(size - resX);
        //body.indices.emplace_back(size - (2*resX));
        //body.indices.emplace_back(size - (resX - 1));

        //body.indices.emplace_back(size - resX);
        //body.indices.emplace_back(size - (resX - 1));
        //body.indices.emplace_back(size);
        theta = 0.0f;
        x = 0;
        y++;
    }

}

camera::camera(int& width, int& height, float& sensitivity, float& speed, float& fov){
    this->last_x = width/2;
    this->last_y = height/2;
    this->pitch = 0.0f;
    this->yaw = -90.0f;
    this->sensitivity = sensitivity;
    this->speed = speed;
    this->fov = fov;
    Δtime = 0.0f;
    last_frame = 0.0f;
    this->pos = glm::vec3(0.0f, 0.0f,  3.0f);
    this->front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->up = glm::vec3(0.0f, 1.0f,  0.0f);
    this->view = glm::translate(view, pos);
    this->projection = glm::perspective(fov, (float)width/(float)height, 0.0001f, 100.0f);
}

void camera::translate(GLFWwindow* window){
    float frame = glfwGetTime();
    Δtime = frame - last_frame;
    last_frame = frame;
    float camera_speed = speed * Δtime;
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        pos += camera_speed * up;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS){
        pos -= camera_speed * up;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        pos += camera_speed * front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        pos -= camera_speed * front;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        pos -= glm::normalize(glm::cross(front, up)) * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        pos += glm::normalize(glm::cross(front, up)) * camera_speed;
    }
    view = glm::lookAt(pos, pos + front, up);
}

void camera::mouse(GLFWwindow* window, double xpos, double ypos){
    if (first){
        last_x = xpos;
        last_y = ypos;
        first = false;
    }
    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos; // reversed since y-coordinates range from bottom to top
    last_x = xpos;
    last_y = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw   += xoffset;
    pitch += yoffset;
    if(pitch > 89.0f){
        pitch =  89.0f;
    }
    if(pitch < -89.0f){
        pitch = -89.0f;
    }
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}

static camera* cam = nullptr;

void mouser(GLFWwindow* window, double xpos, double ypos) {
    cam->mouse(window, xpos, ypos);
}

//Scales is useless rn. If you want it to actually do smt, make it change the size of each model
scene_3d::scene_3d(int& width, int& height, int& fps, std::vector<glm::vec3>& positions, std::vector<float>& scales, float sensitivity, float speed, float fov, std::string image, std::vector<glm::vec4>& color, std::string program, int resX, int resY, float radius){
    this->fps = fps;
    this->width = width;
    this->height = height;

    //Opengl and window setup.
    if (!glfwInit())
        std::cerr << "Uh oh.\n";
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);


    window = glfwCreateWindow(width, height, "SIMULATION", NULL, NULL);


    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        std::cerr << "Error!\n";
    }

    std::cout << glGetString(GL_VERSION) << "\n";

    glEnable(GL_DEPTH_TEST);

    this->exposure = std::make_unique<camera>(width, height, sensitivity, speed, fov);
    cam = exposure.get();
    for(int i = 0; i < colors.size(); i++){
        this->colors.emplace_back(colors[i]);
    }
    for(int i = 0; i < positions.size(); i++){
        this->positions.emplace_back(positions[i]/scales[i]);
    }
    this->colors = color;

    mesh.generate(resX, resY, radius);
    this->vb = std::make_unique<v_buffer>(this->mesh.body.points.size()*sizeof(float), this->mesh.body.points.data());
    arrayLayout layout;
    layout.push(3, GL_FLOAT);
    //layout.push(2, GL_FLOAT);
    this->va = std::make_unique<v_array>();
    this->va->addBuffer(*this->vb, layout);
    this->ib = std::make_unique<i_buffer>(this->mesh.body.indices.size(), this->mesh.body.indices.data());
    this->program = std::make_unique<shader>(program);
    this->image = std::make_unique<texture>(image);
    this->r = std::make_unique<renderer>();

    //Disable mouse visibility and movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Light
    this->light_color = {1.0f, 1.0f, 1.0f, 1.0f};
    this->light_pos = {-20.0f, 10.0f, 0.0f};

    //Uniforms
    this->program->bind();
    this->program->setuniformMat4f("projection", this->exposure->projection);
    this->program->setuniform3f("lightPos", this->light_pos);
    this->program->setuniform4f("u_LightColor", this->light_color);
    this->image->bind();
}

void scene_3d::run(bool video, bool screen, bool texture){
    this->program->setuniform1i("u_istext", texture);
    if(video){
        std::string cmd_str = "ffmpeg -r " + std::to_string(fps) + " -f rawvideo -pix_fmt rgba -s " + std::to_string(width) + "x" + std::to_string(height) + " -i - -threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip ../videos/simulation.mp4 -pix_fmt yuv420p";
        this->cmd = cmd_str.c_str();

        this->ffmpeg = popen(cmd, "w");
        buffer = new int[width*height];
    }
    
    while(!glfwWindowShouldClose(window)){
        glfwSetCursorPosCallback(window, mouser);

        exposure->translate(window);

        r->Clear();
        CALL(glClear(GL_DEPTH_BUFFER_BIT));

        program->setuniformMat4f("view", exposure->view);


        for(int i = 0; i < positions.size(); i++){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);
            program->setuniformMat4f("model", model);
            program->setuniform4f("u_Color", colors[i]);
            
            r->Draw(*va, *ib, *program);
        }

        if(video){
            glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            fwrite(buffer, sizeof(int)*width*height, 1, ffmpeg);
        }

        if(screen){
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    if(video) pclose(ffmpeg);
}