#version 330 core
out vec4 FragColor;

// �߱� ������ ���������� �޽��ϴ�.
uniform vec3 emission;
uniform vec3 objectColor;
void main(){
    // �߱� ������ ����Ͽ� ������ ��� ���̵��� ����
    FragColor = vec4(objectColor*emission, 1.0);
}