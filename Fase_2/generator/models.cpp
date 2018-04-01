#include "models.h"

using std::vector;

Shape* create_XY(float w, float h, float l, int d);
Shape* create_XZ(float w, float h, float l, int d);
Shape* create_YZ(float w, float h, float l, int d);

Shape* createPlane(float size){
    Shape* plane = new Shape();
    float vtx = size/2;

    plane->pushVertex(new Vertex(-vtx , 0 , -vtx));
    plane->pushVertex(new Vertex(-vtx , 0 , vtx));
    plane->pushVertex(new Vertex(vtx , 0 , vtx));

    plane->pushVertex(new Vertex(-vtx , 0 , -vtx));
    plane->pushVertex(new Vertex(vtx , 0 , vtx));
    plane->pushVertex(new Vertex(vtx , 0 , -vtx));

    return plane;
}

Shape* createBox(float width, float height, float length, int div){
    Shape *box = new Shape(), *aux;

    // Create XY Front Face
    aux = create_XY(width,height,(length/2),div);
    box->pushShape(aux);

    // Create XY Back Face
    aux = create_XY(width,height,(-length/2),div);
    aux->reverse();
    box->pushShape(aux);

    // Create XZ Top Face
    aux = create_XZ(width,(height/2),length,div);
    box->pushShape(aux);

    // Create XZ Bottom Face
    aux = create_XZ(width,(-height/2),length,div);
    aux->reverse();
    box->pushShape(aux);

    // Create YZ Right Face
    aux = create_YZ((width/2),height,length,div);
    box->pushShape(aux);

    // Create YZ Left Face
    aux = create_YZ((-width/2),height,length,div);
    aux->reverse();
    box->pushShape(aux);

    return box;
}

Shape* create_XY(float w, float h, float l, int d){
    Shape *face = new Shape();
    float x_pos, x_shift;
    float y_pos, y_shift;

    y_pos = h/2;
    if(d==0) y_shift = h;
    else y_shift = h/d;

    x_pos = w/2;
    if(d==0) {x_shift = w; d = 1;}
    else x_shift = w/d;

    for(int i = 0; i < d; i++){
        for(int j = 0; j<d; j++){

            face->pushVertex(new Vertex(-x_pos+(j*x_shift), -y_pos+(i*y_shift), l));
            face->pushVertex(new Vertex((-x_pos + x_shift)+(j*x_shift), -y_pos+(i*y_shift) , l));
            face->pushVertex(new Vertex((-x_pos + x_shift)+(j*x_shift), (-y_pos + y_shift)+(i*y_shift), l));

            face->pushVertex(new Vertex(-x_pos+(j*x_shift), -y_pos+(i*y_shift), l));
            face->pushVertex(new Vertex((-x_pos + x_shift)+(j*x_shift), (-y_pos + y_shift)+(i*y_shift), l));
            face->pushVertex(new Vertex(-x_pos+(j*x_shift), (-y_pos + y_shift)+(i*y_shift), l));
        }
    }

    return face;
}

Shape* create_XZ(float w, float h, float l, int d){
    Shape *face = new Shape();
    float x_pos, x_shift;
    float z_pos, z_shift;

    x_pos = w/2;
    if(d==0) x_shift = w;
    else x_shift =  w/d;

    z_pos = l/2;
    if(d==0) {z_shift = l; d=1;}
    else z_shift = l/d;


    for(int i = 0; i < d; i++){
            for(int j = 0; j < d; j++){
            face->pushVertex(new Vertex(-x_pos + (i*x_shift), h, -z_pos + (j*z_shift)));
            face->pushVertex(new Vertex(-x_pos + (i*x_shift), h, (-z_pos + z_shift)+(j*z_shift)));
            face->pushVertex(new Vertex((-x_pos + x_shift) + (i*x_shift), h,(-z_pos + z_shift) + (j*z_shift)));

            face->pushVertex(new Vertex(-x_pos + (i*x_shift), h, -z_pos + (j*z_shift)));
            face->pushVertex(new Vertex((-x_pos + x_shift)+(i*x_shift), h,(-z_pos + z_shift) + (j*z_shift)));
            face->pushVertex(new Vertex((-x_pos + x_shift)+(i*x_shift), h, -z_pos + (j*z_shift)));
        }
    }

    return face;
}

Shape* create_YZ(float w, float h, float l, int d){
    Shape *face = new Shape();
    float y_pos, y_shift;
    float z_pos, z_shift;

    y_pos = h/2;
    if(d==0) y_shift = h;
    else y_shift = h/d;

    z_pos = l/2;
    if(d==0) {z_shift = l; d=1;}
    else z_shift = l/d;

    for(int i = 0; i<d; i++){
        for(int j = 0; j<d; j++){

            face->pushVertex(new Vertex(w, -y_pos+(i*y_shift) , (-z_pos + z_shift)+(j*z_shift)));
            face->pushVertex(new Vertex(w, -y_pos+(i*y_shift), -z_pos+(j*z_shift)));
            face->pushVertex(new Vertex(w, (-y_pos + y_shift)+(i*y_shift), -z_pos+(j*z_shift)));

            face->pushVertex(new Vertex(w, -y_pos + (i*y_shift) , (-z_pos + z_shift)+(j*z_shift)));
            face->pushVertex(new Vertex(w, (-y_pos + y_shift)+(i*y_shift), -z_pos+(j*z_shift)));
            face->pushVertex(new Vertex(w, (-y_pos + y_shift)+(i*y_shift), (-z_pos + z_shift)+(j*z_shift)));

        }
    }

    return face;
}

Shape* createCone(float radius, float height, int slices, int stacks){
    Shape* cone = new Shape();
    float a = 0;
    float a_shift = (2 * M_PI) / slices;
    float r_shift = radius / stacks;
    float h_init = -height/2;
    float h_shift = height / stacks;

   for(a = 0; a < 2*M_PI; a+=a_shift) {
        cone->pushVertex(new Vertex((radius*sin(a)), h_init, (radius*cos(a))));
        cone->pushVertex(new Vertex(0, h_init, 0));
        cone->pushVertex(new Vertex((radius*sin(a+a_shift)), h_init, (radius*cos(a+a_shift))));
    }

    for(int i = 0; i < stacks; i++){
        for(int j = 0; j < slices; j++){
            a += a_shift;
            float new_r = radius - r_shift;

            cone->pushVertex(new Vertex((radius*sin(a)), h_init, (radius*cos(a))));
            cone->pushVertex(new Vertex((radius*sin(a+a_shift)), h_init, (radius*cos(a+a_shift))));
            cone->pushVertex(new Vertex((new_r*sin(a)), (h_init + h_shift), (new_r*cos(a))));

            cone->pushVertex(new Vertex((radius*sin(a+a_shift)), h_init, (radius*cos(a+a_shift))));
            cone->pushVertex(new Vertex((new_r*sin(a+a_shift)), (h_init + h_shift), (new_r*cos(a+a_shift))));
            cone->pushVertex(new Vertex((new_r*sin(a)), (h_init + h_shift), (new_r*cos(a))));
        }

        radius -= r_shift;
        h_init += h_shift;
    }

    return cone;
}

Shape* createSphere(float radius, int slices, int stacks){
    Shape* sphere = new Shape();
    float a = 0, h = 0;
    float a_shift = (2 * M_PI)/slices;
    float h_shift = M_PI/stacks;


    for(int i = 0; i < stacks; i++){
        a = 0;

        for(int j = 0; j < slices;j++){

            sphere->pushVertex(new Vertex((radius*sin(a)*sin(h)),
                                                                           (radius*cos(h)),
                                                                           (radius*cos(a)*sin(h))));

            sphere->pushVertex(new Vertex((radius*sin(a+a_shift)*sin(h+h_shift)),
                                                                           (radius*cos(h+h_shift)),
                                                                           (radius*cos(a+a_shift)*sin(h+h_shift))));

            sphere->pushVertex(new Vertex((radius*sin(a+a_shift)*sin(h)),
                                                                           (radius*cos(h)),
                                                                           (radius*cos(a+a_shift)*sin(h))));


            sphere->pushVertex(new Vertex((radius*sin(a)*sin(h)),
                                                                           (radius*cos(h)),
                                                                           (radius*cos(a)*sin(h))));

            sphere->pushVertex(new Vertex((radius*sin(a)*sin(h+h_shift)),
                                                                           (radius*cos(h+h_shift)),
                                                                           (radius*cos(a)*sin(h+h_shift))));

            sphere->pushVertex(new Vertex((radius*sin(a+a_shift)*sin(h+h_shift)),
                                                                           (radius*cos(h+h_shift)),
                                                                           (radius*cos(a+a_shift)*sin(h+h_shift))));

            a= (j+1) * a_shift;
        }

        h= (i+1) * h_shift;
    }

    return sphere;
}

Shape* createCylinder(float radius, float height, int slices, int stacks){
    Shape* cylinder = new Shape();
    float a = 0;
    float a_shift = (2 * M_PI) / slices;
    float h_init = -height/2;
    float h_shift = height / stacks;


    // Cálculo dos vértices da Base
    for(a = 0; a < 2*M_PI; a+=a_shift) {
        cylinder->pushVertex(new Vertex((radius * sin(a)), h_init, (radius * cos(a))));
        cylinder->pushVertex(new Vertex(0, h_init, 0));
        cylinder->pushVertex(new Vertex((radius * sin(a + a_shift)), h_init, (radius * cos(a + a_shift))));
    }

    // Cálculo dos vértices Laterais do Cilindro, desde a Base até ao Topo
    for(int i = 0; i < stacks; i++){
        for(int j = 0; j < slices; j++){
            a = j*a_shift;

            cylinder->pushVertex(new Vertex((radius*sin(a)), h_init, (radius*cos(a))));
            cylinder->pushVertex(new Vertex((radius*sin(a+a_shift)), h_init, (radius*cos(a+a_shift))));
            cylinder->pushVertex(new Vertex((radius*sin(a)), (h_init + h_shift), (radius*cos(a))));

            cylinder->pushVertex(new Vertex((radius*sin(a+a_shift)), h_init, (radius*cos(a+a_shift))));
            cylinder->pushVertex(new Vertex((radius*sin(a+a_shift)), (h_init + h_shift), (radius*cos(a+a_shift))));
            cylinder->pushVertex(new Vertex((radius*sin(a)), (h_init + h_shift), (radius*cos(a))));
        }

        h_init += h_shift;
    }

    h_init = height/2;

    // Cálculo dos vértices da Base do Cilindro
    for(a = 0; a < 2*M_PI; a+=a_shift) {
        cylinder->pushVertex(new Vertex((radius*sin(a+a_shift)), h_init, (radius*cos(a+a_shift))));
        cylinder->pushVertex(new Vertex(0, h_init, 0));
        cylinder->pushVertex(new Vertex((radius*sin(a)), h_init, (radius*cos(a))));

    }
    return cylinder;
}

Shape* createTorus(float distance, float radius, int slices, int stacks){
    Shape* torus = new Shape();

    float theta = 0;
    float phi = 0;
    float theta_shift = (2*M_PI)/slices;
    float phi_shift = (2*M_PI)/stacks;

    for(int i = 0; i < slices; i++){
        for(int j = 0; j < stacks; j++){
            torus->pushVertex(new Vertex((cos(theta)*(distance + radius * cos(phi))), (sin(theta)*(distance + radius * cos(phi))), (radius*sin(phi))));
            torus->pushVertex(new Vertex((cos(theta + theta_shift)*(distance + radius * cos(phi))), (sin(theta + theta_shift)*(distance + radius * cos(phi))), (radius*sin(phi))));
            torus->pushVertex(new Vertex((cos(theta + theta_shift)*(distance + radius * cos(phi + phi_shift))), (sin(theta+theta_shift)*(distance + radius * cos(phi+phi_shift))), (radius*sin(phi+phi_shift))));

            torus->pushVertex(new Vertex((cos(theta + theta_shift)*(distance + radius * cos(phi + phi_shift))), (sin(theta+theta_shift)*(distance + radius * cos(phi+phi_shift))), (radius*sin(phi+phi_shift))));
            torus->pushVertex(new Vertex((cos(theta)*(distance + radius * cos(phi + phi_shift))), (sin(theta)*(distance + radius * cos(phi + phi_shift))), (radius*sin(phi + phi_shift))));
            torus->pushVertex(new Vertex((cos(theta)*(distance + radius * cos(phi))), (sin(theta)*(distance + radius * cos(phi))), (radius*sin(phi))));

            phi = phi_shift * (j + 1);
        }
        theta = theta_shift * (i + 1);
    }
//torus 5 3 20 20 torus.3d
    return torus;
}

// Polyhedrons

//Tetra
//Octa
//Dodeca
// Ico