#version 330 core
out vec4 FragColor;

// 발광 색상을 유니폼으로 받습니다.
uniform vec3 emission;
uniform vec3 objectColor;
void main(){
    // 발광 색상만을 사용하여 코인이 밝게 보이도록 설정
    FragColor = vec4(objectColor*emission, 1.0);
}