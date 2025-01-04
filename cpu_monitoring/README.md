# CPU Avg
단말에서 사내 솔루션의 성능 측정을 목적으로 CPU 부하를 체크해야되는 상황에 사용하고자 작성 


단말에서 top 명령어를 통해 CPU load를 output.txt에 저장하고 이를 로컬에서 py 코드로 DMIPS, CPU Load Avg 계산하는 간단한 코드


output.txt에 top 프로세스의 출력이 기록되어 있어야 하며 각 단말에서 top 명령어 입력 시 CPU가 출력되는 index를 알아야 한다.

코드 상에 DMIPS, CPU_IDX의 수동 입력이 필요하다.