// Copyright (C) 2010-2011 by
// Laboratoire de Biochimie Theorique (CNRS),
// Laboratoire d'Informatique Fondamentale d'Orleans (Universite d'Orleans), (INRIA) and
// Departement des Sciences de la Simulation et de l'Information (CEA). 

// License: CeCILL-C license (http://www.cecill.info/)

// Contact: Marc Baaden
// E-mail: baaden@smplinux.de
// Webpage: http://hyperballs.sourceforge.net

#ifndef __STICKIMPROVED_FRAG_H__
#define __STICKIMPROVED_FRAG_H__
const char stickimproved_frag[] = ""
"\n"
"\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"\n"
"\n"
"struct Ray {\n"
"   vec3 origin ;\n"
"   vec3 direction ;\n"
"};\n"
"\n"
"\n"
"varying mat4  	matrix_near;\n"
"varying vec4  		color_atom1;\n"
"varying vec4  		color_atom2;\n"
"varying float   	shrink;\n"
"\n"
"varying vec4 		prime1;\n"
"varying vec4		prime2;\n"
"\n"
"vec4 lit(float NdotL, float NdotH, float m) {\n"
"\n"
"  float ambient = 1.0;\n"
"  float diffuse = max(NdotL, 0.0);\n"
"  float specular = pow(NdotH,m);\n"
"  if(NdotL < 0.0 || NdotH < 0.0)\n"
"  	specular = 0.0;\n"
"\n"
"  return vec4(ambient, diffuse, specular, 1.0);\n"
"}\n"
"\n"
"\n"
"bool cutoff_plane (vec3 M, vec3 cutoff, vec3 x3){\n"
"  float a = x3.x;\n"
"  float b = x3.y;\n"
"  float c = x3.z;\n"
"  float d = -x3.x*cutoff.x-x3.y*cutoff.y-x3.z*cutoff.z;\n"
"  float l = a*M.x+b*M.y+c*M.z+d;\n"
"  if (l<0.0) {return true;}\n"
"  else{return false;}\n"
"}\n"
"\n"
"\n"
"vec3 isect_surf(Ray r, mat4 matrix_coef){\n"
"   vec4 direction = vec4(r.direction, 0.0);\n"
"   vec4 origin = vec4(r.origin, 1.0);\n"
"   float a = dot(direction,(matrix_coef*direction));\n"
"   float b = dot(origin,(matrix_coef*direction));\n"
"   float c = dot(origin,(matrix_coef*origin));\n"
"   float delta =b*b-a*c;\n"
"   if (delta<0.0) discard;\n"
"   float t1 =(-b-sqrt(delta))/a;\n"
"\n"
"   // Second solution not necessary if you don't want\n"
"   // to see inside spheres and cylinders, save some fps\n"
"   //float t2 = (-b+sqrt(delta)) / a  ;\n"
"   //float t =(t1<t2) ? t1 : t2;\n"
"\n"
"   return r.origin+t1*r.direction;\n"
"}\n"
"\n"
"\n"
"Ray primary_ray(vec4 near1, vec4 far1){\n"
"    vec3 near=near1.xyz/near1.w;\n"
"    vec3 far=far1.xyz/far1.w;\n"
"    return Ray(near,far-near);\n"
"}\n"
"\n"
"\n"
"float update_z_buffer(vec3 M, mat4 ModelViewP){\n"
"    float  depth1;\n"
"    vec4 Ms=(ModelViewP*vec4(M,1.0));\n"
"    return depth1=(1.0+Ms.z/Ms.w)/2.0;\n"
"}\n"
"\n"
"\n"
"void main()\n"
"{\n"
"\n"
"vec4 i_near, i_far, focus;\n"
"vec3 e3, e1, e1_temp, e2;\n"
"\n"
"i_near = vec4(matrix_near[0][0],matrix_near[0][1],matrix_near[0][2],matrix_near[0][3]);\n"
"i_far  = vec4(matrix_near[1][0],matrix_near[1][1],matrix_near[1][2],matrix_near[1][3]);\n"
"focus = vec4(matrix_near[2][0],matrix_near[2][1],matrix_near[2][2],matrix_near[2][3]);\n"
"e3 = vec3(matrix_near[3][0],matrix_near[3][1],matrix_near[3][2]);\n"
"\n"
"e1.x = 1.0;\n"
"e1.y = 1.0;\n"
"e1.z = ( (e3.x*focus.x + e3.y*focus.y + e3.z*focus.z) - e1.x*e3.x - e1.y*e3.y)/e3.z;\n"
"e1_temp = e1 - focus.xyz;\n"
"e1 = normalize(e1_temp);\n"
"\n"
"e2 = normalize(cross(e1,e3));\n"
"\n"
"\n"
"vec4 equation = focus;\n"
"\n"
"float shrinkfactor = shrink;\n"
"float t1 = -1.0/(1.0-shrinkfactor);\n"
"float t2 = 1.0/(shrinkfactor);\n"
"\n"
"vec4 colonne1, colonne2, colonne3, colonne4;\n"
"mat4 mat;\n"
"\n"
"vec3 equation1 = vec3(t2,t2,t1);\n"
"\n"
"\n"
"  float A1 = - e1.x*equation.x - e1.y*equation.y - e1.z*equation.z;\n"
"  float A2 = - e2.x*equation.x - e2.y*equation.y - e2.z*equation.z;\n"
"  float A3 = - e3.x*equation.x - e3.y*equation.y - e3.z*equation.z;\n"
"\n"
"  float A11 = equation1.x*e1.x*e1.x +  equation1.y*e2.x*e2.x + equation1.z*e3.x*e3.x;\n"
"  float A21 = equation1.x*e1.x*e1.y +  equation1.y*e2.x*e2.y + equation1.z*e3.x*e3.y;\n"
"  float A31 = equation1.x*e1.x*e1.z +  equation1.y*e2.x*e2.z + equation1.z*e3.x*e3.z;\n"
"  float A41 = equation1.x*e1.x*A1   +  equation1.y*e2.x*A2   + equation1.z*e3.x*A3;\n"
"\n"
"  float A22 = equation1.x*e1.y*e1.y +  equation1.y*e2.y*e2.y + equation1.z*e3.y*e3.y;\n"
"  float A32 = equation1.x*e1.y*e1.z +  equation1.y*e2.y*e2.z + equation1.z*e3.y*e3.z;\n"
"  float A42 = equation1.x*e1.y*A1   +  equation1.y*e2.y*A2   + equation1.z*e3.y*A3;\n"
"\n"
"  float A33 = equation1.x*e1.z*e1.z +  equation1.y*e2.z*e2.z + equation1.z*e3.z*e3.z;\n"
"  float A43 = equation1.x*e1.z*A1   +  equation1.y*e2.z*A2   + equation1.z*e3.z*A3;\n"
"\n"
"  float A44 = equation1.x*A1*A1 +  equation1.y*A2*A2 + equation1.z*A3*A3 - equation.w;\n"
"\n"
"  colonne1 = vec4(A11,A21,A31,A41);\n"
"  colonne2 = vec4(A21,A22,A32,A42);\n"
"  colonne3 = vec4(A31,A32,A33,A43);\n"
"  colonne4 = vec4(A41,A42,A43,A44);\n"
"\n"
"  mat = mat4(colonne1,colonne2,colonne3,colonne4);\n"
"\n"
"\n"
"\n"
" // Ray calculation using near and far\n"
" Ray ray = primary_ray(i_near,i_far) ;\n"
"\n"
" // Intersection between ray and surface for each pixel\n"
" vec3 M;\n"
" M = isect_surf(ray, mat);\n"
"\n"
" // Recalculate the depth in function of the new pixel position\n"
" gl_FragDepth = update_z_buffer(M, gl_ModelViewProjectionMatrix) ;\n"
"\n"
" // cut the extremities of bonds to superimpose bond and spheres surfaces\n"
" if (cutoff_plane(M, prime1.xyz, -e3) || cutoff_plane(M, prime2.xyz, e3)){discard;}\n"
"\n"
"\n"
"  // Transform normal to model space to view-space\n"
"  vec4 M1 = vec4(M,1.0);\n"
"  vec4 M2 =  mat*M1;\n"
"  vec3 normal = normalize((gl_ModelViewMatrixInverseTranspose*M2).xyz);\n"
"\n"
"  // Give light vector position perpendicular to the screen\n"
"  vec3 lightvec = normalize(vec3(0.0,0.0,1.2));\n"
"  vec3 eyepos = vec3(0.0,0.0,1.0);\n"
"\n"
"  // calculate half-angle vector\n"
"  vec3 halfvec = normalize(lightvec + eyepos);\n"
"\n"
"  // Parameters used to calculate per pixel lighting\n"
"  // see http://http.developer.nvidia.com/CgTutorial/cg_tutorial_chapter05.html\n"
"  float diffuse = dot(normal,lightvec);\n"
"  float specular = dot(normal, halfvec);\n"
"  vec4 lighting = lit(diffuse, specular, 32.0);\n"
"\n"
"  // Mix the color bond in function of the two atom colors\n"
"  float distance_ratio = ((M.x-prime2.x)*e3.x + (M.y-prime2.y)*e3.y +(M.z-prime2.z)*e3.z)/distance(prime2.xyz,prime1.xyz);\n"
"  // lerp function not in GLSL. Find something else ...\n"
"  vec3 diffusecolor = mix( color_atom2.xyz, color_atom1.xyz, distance_ratio );\n"
"\n"
"\n"
"\n"
"  vec3 specularcolor = vec3(1.0,1.0,1.0);\n"
"\n"
"  // Give color parameters to the Graphic card\n"
"  gl_FragColor.rgb = lighting.y * diffusecolor + lighting.z * specularcolor;\n"
"  gl_FragColor.a = 1.0;\n"
"\n"
"  	 // ############## Fog effect #####################################################\n"
"	 // To use fog comment the two previous lines: ie  gl_FragColor.rgb = � and   gl_FragColor.a = 1.0;\n"
"	 // and uncomment the next lines.\n"
"	 // Color of the fog: white\n"
"	 //float fogDistance  = update_z_buffer(M, gl_ModelViewMatrix) ;\n"
"  	 //float fogExponent  = fogDistance * fogDistance * 0.007;\n"
"	 //vec3 fogColor   = vec3(1.0, 1.0, 1.0);\n"
"	 //float fogFactor   = exp2(-abs(fogExponent));\n"
"	 //fogFactor = clamp(fogFactor, 0.0, 1.0);\n"
"\n"
"	 //vec3 final_color = lighting.y * diffusecolor + lighting.z * specularcolor;\n"
"	 //gl_FragColor.rgb = mix(fogColor,final_color,fogFactor);\n"
"  	 //gl_FragColor.a = 1.0;\n"
"	 // ##################################################################################\n"
"\n"
"}\n"
;
#endif // __STICKIMPROVED_FRAG_H__

