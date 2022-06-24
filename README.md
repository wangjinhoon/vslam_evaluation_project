# VSLAM 프로젝트


✅ : 채택

- Git [GitHub - s2lam/vslam_project](https://github.com/s2lam/vslam_project/tree/main)

<aside>
💡 아우디의 요구사항                                                                                                                  
우리는 VSLAM으로 주행 중 자동차의 위치 정보를 뽑아서, 우리의 딥러닝 시스템의 Ground truth 정보로 사용하고 싶습니다. ORB-SLAM을 사용하는데, CPU를 너무 많이 사용하기도 하고 속도도 너무 느리더군요.

💡  **적은 계산량으로 빠르게 돌면서 자동차의 pose 정보를 뽑는 알고리즘을 만들어 주세요**


### 프로젝트 설명 및 가이드

[프로젝트_안내.pptx-2.pdf](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8_%EC%95%88%EB%82%B4.pptx-2.pdf)

### 요구사항

- 우리는 VSLAM으로 주행 중 자동차의 위치 정보를 뽑아서, 우리의 딥러닝 시스템의 Ground truth 정보로 사용하고 싶습니다.
- ORB-SLAM을 사용하는데, CPU를 너무 많이 사용하기도 하고 속도도 너무 느리더군요.
- **적은 계산량으로 빠르게 돌면서 자동차의 pose 정보를 뽑는 알고리즘**을 만들어주세요.

### 질문사항

- 연산 속도에 대한 정량적인 지표가 있는지
- cpu 점유율에 대한 정량적인 지표가 있는지
- 알고리즘으로 도출된 pose 값들에 대해 평가할 수 있는 평가지표나 데이터가 있는지
- 카메라 마다 왜곡을 보정할 때의 고유한 설정값(intrinsic matrix)이 다른데 사용되는 값이 있는지, 아니면 신경안써도 되는지
- Pose 정보에 대한 원하는 데이터 형태가 있는지
- Mapping이 필요한지, visualizing 해야 하는지, 아니면 pose 정보만 원하시는지
- Back-end 에서 RANSAC, ProSAC, LoRANSAC 예제코드를 받을 수 있는지

### 보너스포인트

- KITTI 데이터셋에서 돌 수 있게 프레임워크 개량 (+50점)
- PC 웹캠 / 리얼센스 카메라로 실시간 데모가 가능하게 개량 (+100점)
- 자이카에서 돌 수 있게 프레임워크 개량 (+100점)
- 정확도 개선 (+50점)
- 속도 개선 (+50점)
- 오프라인 시각화 가능 (+50점)
- 실시간 시각화 가능 (+100점)
- 아키텍처 / 알고리즘 재사용성 개선 (+100점)
- 안정성 확보 - CI/CD 및 유닛테스트 (+100점)
- 다른팀에게도도움이될수있는자료정리및공유(+50점)
- 오픈소스를참고해직접VSLAM파이프라인을설계및구현–최소2모듈이상변경(+150점)
- 고객의 갑작스러운 요구사항 1 달성 (+100점)
- 고객의 갑작스러운 요구사항 2 달성 (+200점)

### 프로젝트 일정

[프로젝트 일정](https://www.notion.so/100da9ccd5c34b0188eaf95a21f3e494)

### ORB_SLAM2✅

- 요구 사항에 맞는 가벼운 성능의 Feature detector를 사용하기 위해 결정
- 원문 ORB_SLAM2를 기준으로 성능을 점차 향상 시키기로 결정
- Scale-invariant
- Rotation-invariant

[GitHub - Windfisch/ORB_SLAM2: Real-Time SLAM for Monocular, Stereo and RGB-D Cameras, with Loop Detection and Relocalization Capabilities](https://github.com/Windfisch/ORB_SLAM2)

- 실행 장면
    
    ![Untitled](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled.jpeg)
    

### 성능 측정 방법 → Easy profiler✅

[easy_profiler 사용 메뉴얼](https://www.notion.so/easy_profiler-ee8c1674a90f4feaa65175a36ff28252)

### 정확도 시각적 표현 → EVO✅

[EVO(2D map) [https://github.com/MichaelGrupp/evo](https://github.com/MichaelGrupp/evo)](https://www.notion.so/EVO-2D-map-https-github-com-MichaelGrupp-evo-8670069910084a74a161b54f65ffed06) 

### 성능 향상을 위해 적용한 방법

1. **파라미터 변경**✅
    
    [파라미터 변경](https://www.notion.so/e3dde95301554720a0d2b7cba47458c5)
    
2. **OpenMP**✅
    
    [openmp 적용/비적용](https://www.notion.so/openmp-22859f0f7d7c4e569e4557ff265db54b)
    
3. Gaussian blur✅
4. **~~OpenCL~~**
5. **~~BA 부분 최적화~~**

### 중간 결과

[중간발표자료](https://www.notion.so/23a4921bfc3d4e318de3827212bd9270)

### 최종 발표 자료

[최종 발표](https://www.notion.so/59b1213c247b46ac8027bac96388bbc0)

### pure ORB2 성능

- cpu
    
    ![Untitled](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled.png)
    
- Total time
    
    ![Untitled](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%201.png)
    
- sigle tracking time
    
    ![Untitled](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%202.png)
    
- 정확도
    - trajectory
        
        ![Untitled](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%203.png)
        
    - ATE
        
        ![Untitled](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%204.png)
        
    - RPE
        
        ![Untitled](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%205.png)
        

### ORB2에서 개량한 부분

- thresholds 값 조정
    
    defaut value: 12
    
    - thresholds 별 속도 비교
    - bottleneck
    - bottleneck 분석 및 해결과정
    - 갑작스러운 요구사항 1
    - 최적 threshold 도출
    - 성능향상확인
- feature수 제한
    
    default value: 2000
    
    100씩 줄여가며 실행
    
    - 최적 feature 도출과정
    - 성능실험
- OpenMP 사용한 병렬처리
    - 병렬처리 함수선정
    
    ![VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%206.png](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%206.png)
    
     💡 ExtractORB < operator <     ComputeKeyPointsOctTree 에서 시간이 오래 걸리는 것을 확인. Loopclosing 에서도 시간이 걸릴 것으로 판단되어ComputeKeyPointsOctTree, LoopClosing 부분으로 선정
    
    💡 ExtractORB < operator < ComputeKeyPointsOctTree 에서 시간이 오래 걸리는 것을 확인. Loopclosing 에서도 시간이 걸릴 것으로 판단되어ComputeKeyPointsOctTree, LoopClosing 부분으로 선정
    
    - bottleneck (core dump)
        
        ![VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%207.png](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%207.png)
        
         ⚠️ 코어덤프 발생. 연산량이 많아 메모리 management에 문제가 발생한 것으로 판단.
        
        💡 가장 안쪽 for문부터 순차적으로 병렬처리 구간을 늘려 core dump 발생 전까지 설정.
        
    - 성능향상 확인
        - cpu
            
            ![VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%208.png](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%208.png)
            
        - total time
            
            ![VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%209.png](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%209.png)
            
        - single frame time
            
            ![VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2010.png](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2010.png)
            
        - 정확도
            - trajectory 이미지
                
                ![VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2011.png](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2011.png)
                
            - ATE
                
                ![VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2012.png](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2012.png)
                
            - RPE
                
                ![VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2013.png](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2013.png)
                

### pure ORB와 개량 ORB의 성능비교

- cpu
    
    ![VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2014.png](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2014.png)
    
- total time
    
    ![VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2015.png](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2015.png)
    
- single frame time
        
        ![VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2016.png](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2016.png)
        
- trajectory 이미지
        
        ![VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2017.png](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2017.png)
        
- ATE
        
        ![VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2018.png](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2018.png)
        
- RPE
        
        ![VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2019.png](VSLAM%20%E1%84%91%E1%85%B3%E1%84%85%E1%85%A9%E1%84%8C%E1%85%A6%E1%86%A8%E1%84%90%E1%85%B3%20243b621127a4460391c9f45279c2f3c0/Untitled%2019.png)
        

---

### 최종 결과

- 속도 향상 퍼센트
    - 평균 365 → 292 : 20% 향상
