# Filter Program
- DirectX11을 사용해 제작한 filter 프로그램입니다.
 <br>

#### ⬇️ Click IMG to play with youtube ⬇️
[![Filter](http://img.youtube.com/vi/1S0xK0OHBn4/0.jpg)](https://youtu.be/1S0xK0OHBn4?t=0s)
<br>
<br>
<br>

## 사용 기술
- DirectX 11
- C++
- hlsl (Shaer Files)
<br>
<br>

## 프로젝트 개요
 일부 게임에서는 물체에 일정 시간 열을 가하거나 물과 닿으면 물체가 변하게 되는 기능을 사용합니다. 대표적인 예시로 마인크래프트에서는 Cube로 된 흙 Object를 화로에서 일정 시간 열을 가하면 돌 Object로 변하게 됩니다. 이렇게 물체에 열이 가해지는 현상을 숫자나 게이지가 점점 차오르는 UI로 표현하지 않고 Object의 Texture에 변화를 줌으로 직관적으로 기능을 관찰할 수 있도록 프로젝트를 구성했습니다.

 이 프로젝트에서는 물체에 Ice Filter와 Fire Filter 기능을 적용하고,<br>
필터에 따른 물체의 색상 변화를 Render To Texture를 통해 관찰할 수 있습니다.
<br>
<br>
<br>

## 기능 소개
- 방향키 ← →를 눌러 Cube 모델을 좌우로 이동시킬 수 있습니다.
- 숫자 1을 눌러 Cube의 앞에 Ice 필터를 띄울 수 있습니다.
- 숫자 2를 눌러 Cube의 앞에 Fire 필터를 띄울 수 있습니다.
- 숫자 0을 눌러 Cube에 가해지는 필터를 제거할 수 있습니다.
- 필터1이나 필터2가 켜진 상태에서 방향키 ↑↓를 눌러 Filter 효과를 키우거나 줄일 수 있습니다.
- Cube 모델의 위치가 필터에서 크게 벗어나면 필터 효과가 가해지지 않습니다.
- 우측 상단 Render Texture를 통해 모델에 가해지는 Filter 효과를 직관적으로 관찰할 수 있습니다.

## 구현 기능 상세
### (1) - Cube Model Render
![image](https://github.com/user-attachments/assets/bc999eaf-cbb4-48e7-93c1-d05524ed5df0)<br>
cube.txt File를 사용해 네모난 큐브 모델을 로드했습니다.<br>
큐브 모델의 Texture는 block.jpg 리소스를 저장해 프로젝트에 포함했고,<br>
normal map 변환 사이트를 이용해 blocknormal.jpg를 만들어 프로젝트에 포함시켰습니다. 아래는 원본 텍스쳐 이미지입니다.<br>
<br>
(좌) block.jpg  /  (우) blockNormal.jpg<br>
<img src="https://github.com/user-attachments/assets/be4bee62-9199-4367-9832-d017f9272b5e" width="400" height="400"/>
<img src="https://github.com/user-attachments/assets/2d11a242-0696-4064-9400-1bc37712865f" width="400" height="400"/>
<br>


