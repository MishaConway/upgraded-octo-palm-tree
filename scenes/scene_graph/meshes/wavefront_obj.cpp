#include "wavefront_obj.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <unordered_map>



struct WavefrontVertexIndices{
    int position_index_1, position_index_2, position_index_3;
    int normal_index_1, normal_index_2, normal_index_3;
    int texcoord_index_1, texcoor_index_2;
    
    WavefrontVertexIndices(){}
    WavefrontVertexIndices( int p1, int p2, int p3, int n1, int n2, int n3, int t1, int t2 ){
        position_index_1 = p1;
        position_index_2 = p2;
        position_index_3 = p3;
        normal_index_1 = n1;
        normal_index_2 = n2;
        normal_index_3 = n3;
        texcoord_index_1 = t1;
        texcoor_index_2 = t2;
    }
    
};


SceneGraph::Node* NodeTreeFromWavefrontModel( const std::string& filepath ){
    auto root = new SceneGraph::Node;
    
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    
    
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filepath.c_str(), "/", true);
    
    if (!err.empty()) { // `err` may contain warning message.
        printf( "error: %s\n", err.c_str() );
    }
    
    if (!ret) {
        exit(1);
    }
    
    auto indices_per_material = new std::vector<WavefrontVertexIndices>[materials.size()+1];

    
    
    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        for( int material_id = 0; material_id < materials.size()+1; material_id++ )
            indices_per_material[material_id].clear();
        
        
        
        printf( "processing wavefront shape %s\n", shapes[s].name.c_str());

        
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];
            
            auto current_material_id =  shapes[s].mesh.material_ids[f] + 1;

            
            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                
                //vertex indices
                auto position_index_1 = 3*idx.vertex_index+0;
                auto position_index_2 = 3*idx.vertex_index+1;
                auto position_index_3 = 3*idx.vertex_index+2;
                //tinyobj::real_t vx = attrib.vertices[position_index_1];
                //tinyobj::real_t vy = attrib.vertices[position_index_2];
                //tinyobj::real_t vz = attrib.vertices[position_index_3];
                
                //normal indices
                auto normal_index_1 = 3*idx.normal_index+0;
                auto normal_index_2 = 3*idx.normal_index+1;
                auto normal_index_3 = 3*idx.normal_index+2;
                //tinyobj::real_t nx = attrib.normals[normal_index_1];
                //tinyobj::real_t ny = attrib.normals[normal_index_2];
                //tinyobj::real_t nz = attrib.normals[normal_index_3];
                
                //texcoord indices
                auto texcoord_index_1 = 2*idx.texcoord_index+0;
                auto texcoord_index_2 = 2*idx.texcoord_index+1;
                //tinyobj::real_t tx = attrib.texcoords[texcoord_index_1];
                //tinyobj::real_t ty = attrib.texcoords[texcoord_index_2];
                
                //printf( "current_material_id is %i\n", current_material_id);
                //printf( "num materials is %i\n", materials.size());
                
                indices_per_material[current_material_id].push_back(WavefrontVertexIndices(
                                                                    position_index_1, position_index_2, position_index_3,
                                                                    normal_index_1, normal_index_2, normal_index_3,
                                                                    texcoord_index_1, texcoord_index_2 ));
                
                
            

                
                // Optional: vertex colors
                // tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
                // tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
                // tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
            }
            index_offset += fv;
            
            // per-face material
            shapes[s].mesh.material_ids[f];
        
        }
        
     
        for( int material_id = 0; material_id < materials.size()+1; material_id++  ){
            auto indices = indices_per_material[material_id];
            if( indices.empty() )
                continue;
            
            std::reverse(indices.begin(), indices.end());

            
            //auto wavefront_material = materials[material_id];
            std::vector<Vertex> vertices;

            printf( "indices.size is %i\n", indices.size());
            
            for( int i = 0; i < indices.size(); i++ ){
                auto index = indices[i];
                Vertex v;
                
                auto pos_x = attrib.vertices[ index.position_index_1 ];
                auto pos_y = attrib.vertices[ index.position_index_2 ];
                auto pos_z = attrib.vertices[ index.position_index_3 ];
                v.position =  GeoFloat3( pos_x, pos_y, pos_z );
                
                printf( "attrib.texcoords.size() is %i\n", attrib.texcoords.size());
                
                if( !attrib.texcoords.empty() ){
                    auto tex_u = attrib.texcoords[ index.texcoord_index_1 ];
                    auto tex_v = attrib.texcoords[ index.texcoor_index_2  ];
                    printf( "setting uvs of %f, %f\n", tex_u, tex_v);
                    v.colorUV = GeoFloat3( tex_u, tex_v, 0.0f );
                }
                   
                if( !attrib.normals.empty() ){
                    auto norm_x = attrib.normals[ index.normal_index_1 ];
                    auto norm_y = attrib.normals[ index.normal_index_2 ];
                    auto norm_z = attrib.normals[ index.normal_index_3 ];
                    v.normal = GeoFloat3( norm_x, norm_y, norm_z );
                }
                    
                vertices.push_back(v);
            }
            
            auto node = new SceneGraph::Geode;
            //node->textures["diffuse"] = SceneGraph::TextureDetails( wavefront_material.diffuse_texname );
            node->textures["diffuse"] = SceneGraph::TextureDetails( "metal1.jpg" );
            printf( "vertices.size is %i\n", vertices.size() );
            node->vertex_buffer = OpenGL::VertexBuffer<Vertex>(vertices);
            node->local_transform = GeoMatrix::Scaling( 0.001f );
            node->shader_program = "phong";
            root->children.push_back(node);
            
        }
        
    }
    
    
    printf( "root.children size is %i\n", root->children.size() );
    
    delete [] indices_per_material;

    
    return root;
}