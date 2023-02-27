# ray-tracing_visualize

Visualize ray-tracing by representing sampling ray with line.

- Trace light ray (shadow ray) to check whether this fragment can reach the light source.
- Set object's material : 

    - Phong's color : compute local color (+light source color)
    
    - Flectiont&fraction coeffiecent : determines the weight of color from reflect&refract ray
    
    - Index : Snell's law
- Continuously draw ray.


## Further Improvement
- Performance : should not be memoryless (trace ray every frame).  
- Realtime user interaction : ray origin, object material&position.
- Text label : display each ray properties.
- Problem with no-thickness surface



### Scene 1 :
- Point light + shadow map + cube with reflect&refract property, max ray depth = 2.
- Black light ray means can not reach the light source.
- White sampling ray mens intersect with on one.

![ray_scene0](https://user-images.githubusercontent.com/60053627/221558490-f868a59d-29c9-41d8-aedb-e62a96ac9508.png)

### Scene 2 :
- Point light + 3 mirrors + opaque object (the yellow one), max ray depth = 6.
- Draw object in wireframe mode for displaying purpose.
- All refract rays are white.

https://user-images.githubusercontent.com/60053627/221569432-35df00ac-e101-42a9-a542-db48b9c602c9.mp4

### Scene 3 :
- Point light + sphere with reflect&refract property, max ray depth = 2.
- Color of light ray represents the color from Phong's model from that light source.
- Also, color of sampling ray represents color from it's children.

https://user-images.githubusercontent.com/60053627/221569440-38c4fbd6-c27a-447f-999c-eb8171ce75ac.mp4

