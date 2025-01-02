# Shader Filter Effect
- DirectX11을 사용해 제작한 filter 프로그램입니다.
- README에는 코드 설명, System Flow이 포함되지 않고 저화질 이미지가 나타납니다.
- 따라서 상세한 프로젝트 설명은 프로젝트 설명서 PDF 파일을 참고해 주세요
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
<br>
<br>
### (2) - Cube Normal Texture 적용
가져온 두 이미지를 사용해 PixelShader를 작성했습니다.<br>
blockNormal과 bumpMap을 샘플링을 통해 texture의 Value를 가져온 후 bumpMap의 normal 계산을 위해 값의 범위를 –1~1로 변환합니다.<br> 
이 데이터를 통해 bumpMap의 최종 normal 값을 계산합니다.
<br>
<br>
<br>
### (03) - 사용자 입력을 통한 Cube 모델 이동
방향키 ← → 버튼을 눌러서 모델을 좌우로 이동시킬 수 있습니다.<br>
applicationclass에 m_cubePosX 변수를 선언하고 inputclass에 좌우 방향키가 눌렸다면 true를 반환하는<br>
IsLeftArrowPressed(), IsRightArrowPressed() 함수를 작성했습니다.<br>
applicationclass에서 해당 함수의 true 값을 받으면 m_cubePosX의 값을 증가 감소시키고,<br>
이 값을 Render() 함수에 전달하여 worldMatrix의 x값을 이동시켜 모델 움직임을 구현했습니다.<br>
<br>
(좌) 왼쪽으로 이동 / (우) 오른쪽으로 이동<br>
<img src="https://github.com/user-attachments/assets/4d2f43e2-d12e-4619-a373-c8bcd5e612b1" width="400" height="300"/>
<img src="https://github.com/user-attachments/assets/6ece9366-4b8e-41b5-acb0-cb16f1652e7f" width="400" height="300"/>
<br>
<br>
<br>
### (04) - Cube에 가해지는 두 개의 조명 구현
Cube 모델에 두 개의 조명이 비춰지도록 m_Light1과 m_Light2를 선언 후 초기화했습니다.<br>
Light1은 붉은색을 띄고 ↙방향을 비추도록 Direction을 설정했습니다.<br>
Light2는 푸른색을 띄고 ↘방향을 비추도록 Direction을 설정했습니다.<br>
<br>
두 조명은 Cube에 적용된 shader인 m_NormalMapShader를 통해 normalPS의 상수 버퍼로 전달되며<br>
전달된 상수 버퍼 값을 사용해 Pixel의 최종 색상을 결정합니다.<br>
<br>
아래는 Cube가 왼쪽으로 이동하여 우측 면에 붉은 조명이 보이고<br>
Cube가 오른쪽으로 이동하여 좌측면에 푸른 조명이 보이는 이미지입니다.<br>
중간 부분은 두 조명이 합쳐져 보라색 색상이 나타납니다.<br>
<img src="https://github.com/user-attachments/assets/498b9e0c-34e8-4e50-bc95-996974031a6c" width="400" height="300"/>
<img src="https://github.com/user-attachments/assets/cda26487-b3fa-4273-b8cd-517c0ffd6f15" width="400" height="300"/>
<br>
<br>
<br>
### (05) - 사용자 입력을 통한 Filter 효과(1) - Ice Filter (Render To Texture)
숫자 1을 눌러 Ice Filter를 화면에 띄우고 모델에 효과를 적용할 수 있습니다.<br>
<br>
Ice Filter가 적용된 상태에서 방향키 ↑↓를 눌러 필터의 강도를 높이고 줄일 수 있습니다.<br>
Refraction Scale은 0.0~0.3 범위로 제한됩니다(기본 0.02).<br>
또한, 필터를 적용한 상태에서도 모델을 좌우로 움직일 수 있으며 필터 밖으로 벗어난 Cube 모델은 필터의 효과를 받지 않습니다.<br>
<br>
(좌) refraction scale = 0.0  /  (중) refraction scale = 0.02(기본)  /  (우) refraction scale = 0.3<br>
<img src="https://github.com/user-attachments/assets/38526505-1d4e-4af0-953b-b554783dfde7" width="200" height="150"/>
<img src="https://github.com/user-attachments/assets/0e050243-b80a-40f1-9fdf-ebf8a8d854ab" width="200" height="150"/>
<img src="https://github.com/user-attachments/assets/3787f5ec-88a3-468a-a283-13ec77809e48" width="200" height="150"/>
<img src="https://github.com/user-attachments/assets/2b231eae-f668-4c05-a3b1-84f40183e5a4" width="400" height="300"/>
<img src="https://github.com/user-attachments/assets/bb74f44d-0f06-4cca-bf67-2f060914907f" width="400" height="300"/>
<br>
<br>
<br>
### (06) - 사용자 입력을 통한 Filter 효과(2) - Fire Filter (Alpha Texture)
숫자 2를 눌러 Fire Filter를 화면에 띄우고 모델에 효과를 적용할 수 있습니다.<br>
Fire Filter가 적용된 상태에서 방향키 ↑↓를 눌러 필터의 강도를 높이고 줄일 수 있습니다.<br>
Fire의 강도(밝기)는 0.8~1.7 범위로 제한됩니다(기본 1.3).<br>
또한, 필터를 적용한 상태에서도 모델을 좌우로 움직일 수 있으며 필터 밖으로 벗어난 Cube 모델은 필터의 효과를 받지 않습니다.<br>
<br>
Fire Filter에 사용된 Texture들은 flame01, flame02, flame alpha입니다.<br>
Pixel Shader에서 각 Texture들을 샘플링할 때 y좌표를 감소시킨 후<br>
상수 버퍼로 전달받은 texture translation 값과의 연산을 통해 용암이 흐르는 듯한 효과를 구현했습니다.<br>
<br>
(좌) fire bright = 1.7  /  (중) fire bright = 1.3(기본)  /  (우) fire bright = 0.8<br>
<img src="https://github.com/user-attachments/assets/5195f8bd-545b-4bdf-b64c-6bcd79f6a3ce" width="200" height="150"/>
<img src="https://github.com/user-attachments/assets/57704d79-fa63-4ade-ad5c-d3b4371f0f44" width="200" height="150"/>
<img src="https://github.com/user-attachments/assets/5da0412d-b5c4-4f4b-9dbb-cfa98f6737a3" width="200" height="150"/>
<img src="https://github.com/user-attachments/assets/786e44f8-7975-4761-b2a4-1c436e1ac8ae" width="400" height="300"/>
<img src="https://github.com/user-attachments/assets/798420f0-0ae6-42f3-8585-e9df5fdfb8c2" width="400" height="300"/>
<br>
<br>
<br>
아래는 원본 이미지입니다.<br>
<br>
(좌) flame01.jpg  /  (중) flame02.jpg  /  (우) flameAlpha.png<br>
<img src="https://github.com/user-attachments/assets/201f103e-06d4-44ea-901b-4a8a81bd5b5b" width="200" height="200"/>
<img src="https://github.com/user-attachments/assets/9e9b2464-7235-41b0-ad80-234fc2763ed8" width="200" height="200"/>
<img src="https://github.com/user-attachments/assets/0f505be0-73fe-4cc0-82a0-945cff5649db" width="200" height="200"/>
<br>
<br>
<br>
### (07) - 사용자 입력을 통한 Filter 효과(3) - Filter 제거
숫자 0을 눌러 현재 적용된 필터를 제거합니다.<br>
Ice Filter 효과나 Fire Filter 효과가 적용된 상태에서 숫자 0을 누르면 현재 적용된 필터가 제거됩니다.<br>
아래는 순서대로 숫자1, 숫자2, 숫자0을 누른 후의 모습입니다.<br>
<img src="https://github.com/user-attachments/assets/0195793c-bfd7-464b-9baf-f913f146dd32" width="200" height="150"/>
<img src="https://github.com/user-attachments/assets/34e8f25c-f1d5-4543-8544-ff387797f221" width="200" height="150"/>
<img src="https://github.com/user-attachments/assets/97938469-b8f1-4e64-807a-48fcb12f8156" width="200" height="150"/>
<br>
<br>
<br>
### (08) - Cube Model 관찰을 위한 Render To Texture
화면의 우측 상단에 Render To Texture가 나타나며, 이를 통해 각 필터를 적용함으로 인해 변화하는 Cube의 Texture 변화를 관찰할 수 있습니다.<br>
Filter를 적용한 상태에서는 Filter에 모델이 가려지기 때문에 변화를 관찰하기 어렵습니다.<br>
따라서 필터 효과 없이 Render To Texture를 통해 Cube를 렌더합니다.<br>
Render To Texture를 통해 보이는 Cube는 일정한 속도로 회전하며 이를 통해 Cube의 4면 모두를 관찰할 수 있습니다.<br>
<br>
Render To Texture에는 Light1(빨간색), Light2(파란색)이 아닌 LightPhong(하얀색) 조명이 사용됩니다.<br>
Shader에서는 Phong 조명 연산이 수행되며 이 조명은 Render To Texture 내의 모델에만 적용됩니다.<br>
<br>
우상단 RTT를 통해 Phong 조명이 사용된 회전하는 Cube를 관찰<br>
<img src="https://github.com/user-attachments/assets/23813d65-8e24-4767-abe8-6335d0e76a4e" width="600" height="400"/>
<br>
<br>
<br>
### (09) - Render To Texture를 통한 Cube 모델의 변화 관찰
Ice Filter를 적용하면 shift color 변수의 r, g 값을 감소시켜<br>
우측 상단 Render To Texture를 통해 보이는 Cube Model이 점차 파랗게 보이도록 합니다.<br>
Fire Filter 또한 비슷하게 g, b 값을 감소시켜 점차 빨갛게 변하게 됩니다.<br>
<br>
아래는 각 필터를 적용한 후 일정 시간이 지나 Render To Texture의 Cube 모델이 변한 예시 이미지입니다.<br>
Filter를 통해 Cube의 색상이 변하더라도 화면 중앙의 Cube는 색상이 변하지 않고 오직 Render Texture 내부 Cube의 Texture 색상만 변합니다.<br>
또한, 필터를 변경할 땐 원래의 색상으로 돌아간 다음 서서히 색상히 변하도록 하고, 숫자 0을 눌러 Filter를 제거하면 원본 색상을 계속 유지합니다.<br>
<br>
(좌) Ice 적용 후 파랗게 변함  /  (우) Fire 적용 후 빨갛게 변함<br>
<img src="https://github.com/user-attachments/assets/1292078c-c8fe-4ecd-ac36-18b32c991912" width="400" height="300"/>
<img src="https://github.com/user-attachments/assets/25a48f17-703b-4478-baf1-27fa3eb9cf59" width="400" height="300"/>
<br>
<br>
각 Filter에서 방향키 ↑↓를 누름으로 필터의 강도를 변경함에 따라 색상 변화 속도 또한 증가하고 감소합니다.<br>
강도에 따라 m_shiftValue 값이 증감하며 값의 범위는 0.001~0.0005로 제한됩니다.<br>
Input에 따라 변화하는 shiftValue는 m_shiftColor의 색상에 적용되어 Filter에 따라 색상이 변하도록 하고,<br>
m_shiftColor는 Render To Texture의 Cube 모델의 Shader에 상수 버퍼로 전달되어<br>
Texture, Texture Normal, Phong 조명 연산 등을 수행한 최종 Pixel Color에 누적해주어<br>
해당 색상으로 색상이 서서히 변하는 현상을 구현했습니다.<br>
<br>
Filter Mode를 변경할 때 m_shiftColor와 m_shiftValue는 모두 초기 기본 값으로 초기화되어<br>
이전 Filter의 결과가 다른 Filter에 누적되어 영항을 미치지 않도록 초기화 코드를 작성했습니다.<br>
<br>
<br>
<br>
### (10) - Cube의 위치에 따른 Render Texture 변화 제한
사용자가 Cube를 움직여 Filter 밖으로 벗어난 경우 Render To Texture에 가해지는 색상 변화가 일어나지 않습니다.<br>
Cube Position 값이 일정 범위 내에 있고 Filter가 켜진 상태에서만 색상 변화가 일어납니다.<br>
<img src="https://github.com/user-attachments/assets/dd6c66ed-ee92-4d67-afbd-48a607c81992" width="600" height="400"/>



