
#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 color;
  
uniform sampler2D leftTexture;
uniform sampler2D rightTexture;
uniform float winWidth, winHeight;

uniform int lrX, lrY, lrZ, rrX, rrY, rrZ; //tuning by rotating camera on X, Y, Z  axis

void main()
{
	//color = texture(ourTexture, TexCoord);
	float rx, ry, rz;

    //根据例子改造后的代码
    vec2 pfish;
    float theta,phi,r;
    vec3 psph;
    float FOV = 3.141592654 * 2.2/1.8; // FOV of the fisheye, eg: 180, 220 degrees

	//0, 1/4, 3/4, 1
    float halfWidth = winWidth / 2;
    bool isLeft = (gl_FragCoord.x < (winWidth * 3 / 4)) && (gl_FragCoord.x >= (winWidth / 4));

    ivec2  size;
    if (isLeft) {
        size = textureSize(leftTexture, 0);//纹理图片大小
		rx = 3.141592654 * lrX / 180.0f;
		ry = 3.141592654 * lrY / 180.0f;
		rz = 3.141592654 * lrZ / 180.0f;
    } else {
        size = textureSize(rightTexture, 0);//纹理图片大小
		rx = 3.141592654 * rrX / 180.0f;
		ry = 3.141592654 * rrY / 180.0f;
		rz = 3.141592654 * rrZ / 180.0f;
    }

    float width = size.s;//这里测试用
    float height = size.t;//这里测试用
   
    // Polar angles
    if (isLeft) {
        //下面的代码控制左侧图片
		theta = 2.0 * 3.14159265 * (gl_FragCoord.x / winWidth - 0.5); // -pi to pi
		phi = 3.14159265 * (gl_FragCoord.y / winHeight - 0.5); // -pi/2 to pi/2
    } else{
	   //下面代码控制右侧图片
	    theta = 2.0 * 3.14159265 * (gl_FragCoord.x / winWidth); //  0 to 2pi
	    phi = 3.14159265 * (gl_FragCoord.y / winHeight - 0.5); // -pi/2 to pi/2
    }
   

   theta = theta - rz;
   phi = phi - rx;
      
    // Vector in 3D space
    psph.x = cos(phi) * sin(theta);
    psph.y = cos(phi) * cos(theta);
    psph.z = sin(phi);

    // Calculate fisheye angle and radius
    theta = atan(psph.z, psph.x) + ry;
    phi = atan(sqrt(psph.x*psph.x+psph.z*psph.z),psph.y);
    r = width * phi / FOV;

    // Pixel in fisheye space
    pfish.x = (0.5 * width + r * cos(theta));
    pfish.y = (0.5 * width + r * sin(theta));

    float xoff = pfish.x/width;
    float yoff = pfish.y/height;

    //color = texture2D(leftTexture, TexCoord.st);//原图
	if (isLeft) {
		color = texture2D(leftTexture, vec2(xoff, 1.0f - yoff));
	} else {
		color = texture2D(rightTexture, vec2(xoff, 1.0f - yoff));
	}
}