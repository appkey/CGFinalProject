#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

// �ֿ� ���� ������
uniform vec3 lightPos;
uniform vec3 lightColor;

// ���� ������ ����ü
const int MAX_POINT_LIGHTS = 10;

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform int numPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform vec3 viewPos;       // ī�޶� ��ġ
uniform vec3 objectColor;   // ��ü ����
uniform vec3 emission;      // �߱� ����

uniform bool showNormals;   // ��� �ð�ȭ ����

void main(){
    if(showNormals){
        // ��� �ð�ȭ: ��� ���͸� �������� ����
        vec3 normalizedNormal = normalize(Normal);
        FragColor = vec4(normalizedNormal * 0.5 + 0.5, 1.0); // [-1,1] -> [0,1]
    }
    else{
        // Ambient
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * lightColor;
        
        // Diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        
        // Specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;
        
        // �ֿ� ������ ����� ��ü ���� ����
        vec3 result = (ambient + diffuse + specular) * objectColor;
        
        // ���� �������� ��� �߰�
        for(int i = 0; i < numPointLights; ++i){
            // Diffuse
            vec3 plDir = normalize(pointLights[i].position - FragPos);
            float plDiff = max(dot(norm, plDir), 0.0);
            vec3 plDiffuse = plDiff * pointLights[i].color * pointLights[i].intensity;
            
            // Specular
            vec3 plReflectDir = reflect(-plDir, norm);
            float plSpec = pow(max(dot(viewDir, plReflectDir), 0.0), 32);
            vec3 plSpecular = specularStrength * plSpec * pointLights[i].color * pointLights[i].intensity;
            
            // ���� ���� ���� (�Ÿ� ���)
            float distance = length(pointLights[i].position - FragPos);
            float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
            
            plDiffuse *= attenuation;
            plSpecular *= attenuation;
            
            // �������� Diffuse�� Specular�� ��ü ���� ���� �� �ջ�
            result += (plDiffuse + plSpecular) * objectColor;
        }
        
        // �߱� �߰�
        result += emission;
        
        FragColor = vec4(result, 1.0);
    }
}
