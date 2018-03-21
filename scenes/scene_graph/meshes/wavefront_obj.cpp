#include "wavefront_obj.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <unordered_map>
#include <set>


typedef std::triple<int,int,int> int3;
typedef std::pair<int,int> int2;
typedef std::triple<int3, int3, int2> wavefront_index;

Vertex VertexFromWavefrontIndex( const wavefront_index w_index,
                                const std::vector<tinyobj::real_t>& wavefront_positions,
                                const std::vector<tinyobj::real_t>& wavefront_normals,
                                const std::vector<tinyobj::real_t>& wavefront_texcoords){
    
    const int3 position_indices = w_index.first;
    const int3 normal_indices = w_index.second;
    const int2 texcoord_indices = w_index.third;
    
    Vertex v;
    v.position.x = wavefront_positions[ position_indices.first ];
    v.position.y = wavefront_positions[ position_indices.second ];
    v.position.z = wavefront_positions[ position_indices.third ];
    
    if( !wavefront_normals.empty() ){
        v.normal.x = wavefront_normals[ normal_indices.first ];
        v.normal.y = wavefront_normals[ normal_indices.second ];
        v.normal.z = wavefront_normals[ normal_indices.third ];
    }
    
    if( !wavefront_texcoords.empty() ){
        v.colorUV.x = wavefront_texcoords[ texcoord_indices.first ];
        v.colorUV.y = wavefront_texcoords[ texcoord_indices.second ];
    }

    return v;
}





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
    
    auto indices_per_material = new std::vector<wavefront_index>[materials.size()+1];

    
    
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
                int3 pos_index = int3( position_index_1, position_index_2, position_index_3);
               
                //normal indices
                auto normal_index_1 = 3*idx.normal_index+0;
                auto normal_index_2 = 3*idx.normal_index+1;
                auto normal_index_3 = 3*idx.normal_index+2;
                int3 norm_index = int3( normal_index_1, normal_index_2, normal_index_3 );
                //norm_index = int3(0,0,0);
                
                //texcoord indices
                auto texcoord_index_1 = 2*idx.texcoord_index+0;
                auto texcoord_index_2 = 2*idx.texcoord_index+1;
                int2 tex_index = int2( texcoord_index_1, texcoord_index_2 );
                //tex_index = int2(0,0);

                indices_per_material[current_material_id].push_back( wavefront_index( pos_index, norm_index, tex_index) );
                
            
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
            
            // normalize all indices
            std::set<wavefront_index> normalized_indices;
            for( int i = 0; i < indices.size(); i++ )
                normalized_indices.insert( indices[i] );
            // rank the normalized indices
            auto set_iter = normalized_indices.begin();
            std::map<wavefront_index, unsigned int> ranked_normalized_indices;
            std::vector<Vertex> normalized_vertices;
            for( int i = 0; i < normalized_indices.size(); i++ ){
                wavefront_index w_index = *set_iter;
                ranked_normalized_indices[w_index] = i;
                normalized_vertices.push_back(VertexFromWavefrontIndex(w_index, attrib.vertices, attrib.normals, attrib.texcoords ));
                set_iter++;
            }
            
            
            
            //convert wavefront indices to normalized indices
            std::vector<unsigned int> normalized_face_indices;
            for( int i = 0; i < indices.size(); i++ )
                normalized_face_indices.push_back(ranked_normalized_indices[indices[i]]);
            
            
            std::vector<Vertex> normalized_face_vertices;
            for( int i = 0; i < normalized_face_indices.size(); i++ ){
                unsigned int normalized_face_index = normalized_face_indices[i];
                normalized_face_vertices.push_back( normalized_vertices[normalized_face_index] );
            }
            
                
            
            
            
            
            
            
         
            

            
            //auto wavefront_material = materials[material_id];

            std::vector<Vertex> vertices;
            for( int i = 0; i < indices.size(); i++ ){
                vertices.push_back(VertexFromWavefrontIndex(indices[i], attrib.vertices, attrib.normals, attrib.texcoords));
            }
            
            auto node = new SceneGraph::Geode;
            //node->textures["diffuse"] = SceneGraph::TextureDetails( wavefront_material.diffuse_texname );
            node->textures["diffuse"] = SceneGraph::TextureDetails( "metal1.jpg" );
            printf( "vertices.size is %i\n", vertices.size() );
            printf( "normalzed vertices size is %i\n", normalized_vertices.size() );
            node->vertex_buffer = OpenGL::VertexBuffer<Vertex>(normalized_face_indices, normalized_vertices);
            node->local_transform = GeoMatrix::Scaling( 0.1f );
            node->shader_program = "phong";
            root->children.push_back(node);
            
        }
        
    }
    
    
    delete [] indices_per_material;

    
    return root;
}