// Reference: http://shdr.bkcore.com/#1/lZAxT8MwEIX/yslTClGbiLK0YkJiAzGxUFSZxEldOb7Idlraqv+ds92kUGBgiGL7fXf37h1YiUXXCO0sm72y1ohCWokaVrJetVAp5G6+0BtudlLXsBHFDVRPaBqu6JkElCU0XOpktNCHhYaIlNLAXTgmWZqn2WgOk0noCRpRD1yBCgcyP5M7oRRuPea/4IJ6VlVnBdHjW7iGEl1ycpLSuNHck7VaPhhe35/bTpO+7ipOSyEfZ54+svTPfblzRr53TkSbLVrpiPqp6D6KTsuKzpSF618fOaEf37UpNFgK9SLF9ne5NbgWhR826P8I/6TS6tGC3IvkqxkKIV5jXD4fvxzxF7YIDOH1mw+phYyfT69Ud+mXCqkmxPuWstCVzeLfTmy351qLpQ97vLbs+Ak=

#version 330 core

uniform vec3 lightDirectionReverse = vec3(0.0f, 1.0f, 0.0f);
uniform vec3 color = vec3(1.0f, 1.0f, 1.0f);

in vec3 vertexWorldPosition;
in vec3 vertexWorldNormal;

out vec4 fragmentColor;

void main()
{
	float diffuse = 0.5f + dot(vertexWorldNormal, lightDirectionReverse);
	fragmentColor = vec4(diffuse * color, 1.0f);
}