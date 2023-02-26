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
