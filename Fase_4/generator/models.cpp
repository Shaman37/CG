#include "models.h"

using std::vector;

Shape* create_XY(float w, float h, float l, int d);
Shape* create_XZ(float w, float h, float l, int d);
Shape* create_YZ(float w, float h, float l, int d);

Shape* createPlane(float size){
    Shape* plane = new Shape();
    float vtx = size/2;

    plane->pushVertex(new Vertex(-vtx , 0 , -vtx));
    plane->pushNormal(new Vertex(0,1,0));
    plane->pushTexture(new Vertex(1,1,0));

    plane->pushVertex(new Vertex(-vtx , 0 , vtx));
    plane->pushNormal(new Vertex(0,1,0));
    plane->pushTexture(new Vertex(1,0,0));

    plane->pushVertex(new Vertex(vtx , 0 , vtx));
    plane->pushNormal(new Vertex(0,1,0));
    plane->pushTexture(new Vertex(0,1,0));

    plane->pushVertex(new Vertex(-vtx , 0 , -vtx));
    plane->pushNormal(new Vertex(0,1,0));
    plane->pushTexture(new Vertex(0,1,0));

    plane->pushVertex(new Vertex(vtx , 0 , vtx));
    plane->pushNormal(new Vertex(0,1,0));
    plane->pushTexture(new Vertex(1,0,0));

    plane->pushVertex(new Vertex(vtx , 0 , -vtx));
    plane->pushNormal(new Vertex(0,1,0));
    plane->pushTexture(new Vertex(0,0,0));

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
    Vertex* normal;
    Shape* sphere = new Shape();

    float a = 0, h = 0;
    float a_shift = (2 * M_PI)/slices;
    float h_shift = M_PI/stacks;

    float texU = 1 / (float)slices;
    float texV = 1 / (float)stacks;

    for(int i = 0; i < stacks; i++){
        a = 0;

        for(int j = 0; j < slices;j++){
            float x1 = (radius*sin(a)*sin(h));
            float y1 = (radius*cos(h));
            float z1 = (radius*cos(a)*sin(h));

            float x2 = (radius*sin(a+a_shift)*sin(h+h_shift));
            float y2 = (radius*cos(h+h_shift));
            float z2 = (radius*cos(a+a_shift)*sin(h+h_shift));

            float x3 = (radius*sin(a+a_shift)*sin(h));
            float y3 = (radius*cos(h));
            float z3 = (radius*cos(a+a_shift)*sin(h));

            float  x4 = (radius*sin(a)*sin(h+h_shift));
            float  y4 = (radius*cos(h+h_shift));
            float  z4 = (radius*cos(a)*sin(h+h_shift));

            sphere->pushVertex(new Vertex(x1, y1, z1));
            normal = Vertex::normalCalc(new Vertex(x1,y1,z1));
            sphere->pushNormal(normal);
            sphere->pushTexture(new Vertex(j*texU,i*texV + texV,0));

            sphere->pushVertex(new Vertex(x2, y2, z2));
            normal = Vertex::normalCalc(new Vertex(x2,y2,z2));
            sphere->pushNormal(normal);
            sphere->pushTexture(new Vertex(j*texU + texU,i*texV,0));

            sphere->pushVertex(new Vertex(x3, y3, z3));
            normal = Vertex::normalCalc(new Vertex(x3,y3,z3));
            sphere->pushNormal(normal);
            sphere->pushTexture(new Vertex(j*texU + texU, i*texV + texV, 0));

            sphere->pushVertex(new Vertex(x1, y1, z1));
            normal = Vertex::normalCalc(new Vertex(x1,y1,z1));
            sphere->pushNormal(normal);
            sphere->pushTexture(new Vertex(j*texU,i*texV + texV,0));

            sphere->pushVertex(new Vertex(x4, y4, z4));
            normal = Vertex::normalCalc(new Vertex(x4,y4,z4));
            sphere->pushNormal(normal);
            sphere->pushTexture(new Vertex(j*texU, i*texV, 0));

            sphere->pushVertex(new Vertex(x2,y2,z2));
            normal = Vertex::normalCalc(new Vertex(x2,y2,z2));
            sphere->pushNormal(normal);
            sphere->pushTexture(new Vertex(j*texU + texU, i*texV + texV, 0));

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
    float phi_shift = (2*M_PI)/stacks;
    float theta_shift = (2*M_PI)/slices;


    float textureSide = float(1)/float(stacks);
    float textureRing = float(1)/float(slices);

    for(int i = 0; i < slices; i++){
        double a0 = i*theta_shift;
        double a1 = a0 + theta_shift;

        float x0 = cos(a0);
        float y0 = sin(a0);
        float x1 = cos(a1);
        float y1 = sin(a1);

        for(int j = 0; j < stacks; j++){
            torus->pushVertex(new Vertex((cos(theta)*(distance + radius * cos(phi))), (sin(theta)*(distance + radius * cos(phi))), (radius*sin(phi))));
            torus->pushVertex(new Vertex((cos(theta + theta_shift)*(distance + radius * cos(phi))), (sin(theta + theta_shift)*(distance + radius * cos(phi))), (radius*sin(phi))));
            torus->pushVertex(new Vertex((cos(theta + theta_shift)*(distance + radius * cos(phi + phi_shift))), (sin(theta+theta_shift)*(distance + radius * cos(phi+phi_shift))), (radius*sin(phi+phi_shift))));
            torus->pushNormal(new Vertex( x0*cos(j*phi_shift), y0*cos(j*phi_shift), sin(j*phi_shift)));
            torus->pushNormal(new Vertex( x1*cos(j*phi_shift), y1*cos(j*phi_shift), sin(j*phi_shift)));
            torus->pushNormal(new Vertex( x0*cos((j+1)*phi_shift), y0*cos((j+1)*phi_shift), sin((j+1)*phi_shift)));
            torus->pushTexture(new Vertex(i*textureRing,j*textureSide,0));
            torus->pushTexture(new Vertex((i+1)*textureRing,j*textureSide,0));
            torus->pushTexture(new Vertex(i*textureRing,(j+1)*textureSide,0));

            torus->pushVertex(new Vertex((cos(theta + theta_shift)*(distance + radius * cos(phi + phi_shift))), (sin(theta+theta_shift)*(distance + radius * cos(phi+phi_shift))), (radius*sin(phi+phi_shift))));
            torus->pushVertex(new Vertex((cos(theta)*(distance + radius * cos(phi + phi_shift))), (sin(theta)*(distance + radius * cos(phi + phi_shift))), (radius*sin(phi + phi_shift))));
            torus->pushVertex(new Vertex((cos(theta)*(distance + radius * cos(phi))), (sin(theta)*(distance + radius * cos(phi))), (radius*sin(phi))));
            torus->pushNormal(new Vertex( x0*cos((j+1)*phi_shift), y0*cos((j+1)*phi_shift), sin((j+1)*phi_shift)));
            torus->pushNormal(new Vertex( x1*cos(j*phi_shift), y1*cos(j*phi_shift), sin(j*phi_shift)));
            torus->pushNormal(new Vertex( x1*cos((j+1)*phi_shift), y1*cos((j+1)*phi_shift), sin((j+1)*phi_shift)));
            torus->pushTexture(new Vertex(i*textureRing,(j+1)*textureSide,0));
            torus->pushTexture(new Vertex((i+1)*textureRing,j*textureSide,0));
            torus->pushTexture(new Vertex((i+1)*textureRing,(j+1)*textureSide,0));

            phi = phi_shift * (j + 1);
        }
        theta = theta_shift * (i + 1);
    }

    return torus;
}