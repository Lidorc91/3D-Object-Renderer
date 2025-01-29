# 3D Object Renderer

A custom 3D object renderer built in C++, implementing key rendering techniques such as **wireframe rendering**, **hidden surface removal**, and multiple shading methods (**flat shading**, **Gouraud shading**, and **Phong shading**). The renderer is capable of loading and visualizing `.obj` files using triangle polygons and supports interactive visualization via keyboard controls.

## Features

### 1. **Wireframe Rendering**
- Displays 3D objects using only their edges.
- Ideal for understanding object structure and debugging geometry.

### 2. **Hidden Surface Removal**
- Implements basic visibility determination to render only visible faces, ensuring accurate 3D visualization.

### 3. **Shading Techniques**
This renderer supports three shading techniques:
- **Flat Shading**:
  - Illumination is calculated once per face, using the face's normal and centroid.
  - Produces a faceted, low-poly style of rendering.
- **Gouraud Shading**:
  - Illumination is calculated per vertex and interpolated across the face.
  - Produces smooth color transitions but may miss highlights.
- **Phong Shading**:
  - Illumination is calculated per pixel by interpolating vertex normals.
  - Produces realistic shading with detailed highlights.

### 4. **Light Sources**
- **Ambient Light**: Affects all objects equally, regardless of direction.
- **Directional Light**: Simulates a distant light source (e.g., sunlight) with a specific direction.
- **Point Light**: Simulates a local light source emitting light in all directions.

### 5. **Interactive Controls**
- Switch between shading techniques in real-time using keyboard input.
- Rotate, zoom, and pan objects for a better view.

## Implementation Highlights

- **Scene Organization**:
  - A `Scene` class organizes objects, transformations, and camera.
  - Objects and their transformations are applied consistently across the pipeline.
  
- **Rendering Pipeline**:
  - The pipeline processes vertices through transformations (model, view, projection).
  - Implements rasterization for shading techniques and fills triangles using **barycentric coordinates**.
  
- **Illumination Calculations**:
  - Implements the illumination equation for ambient, diffuse, and specular components.
  - Supports multiple light sources, each affecting object shading dynamically.
