[Github] 그래픽 작업 시 유의사항


1. Project/Content/Graphic 폴더 외 다른 폴더에서 파일 수정, 생성, 삭제 등의 작업은 삼가주세요.
	- 필요한 경우, 소프트 쪽에 바로 연락주세요
	- 특히 소프트 개인 작업 공간에서는 작업을 피해주세요
		- Project/C++ Classes
		- Project/Content/LKH
		- Project/Content/Seohyeon

2. Graphic 폴더 내에서도 가급적이면 개인 폴더를 생성하여, 개인 폴더 내에서만 작업을 진행해주세요.
	- 같은 폴더 내 파일 이름이 겹치거나, 다른 사람이 같은 파일을 작업하면 크러시가 발생해요😭
	- 지속적으로 해당 문제가 발생하면 소프트 쪽에 바로 연락주세요

3. 작업은 개인 리포지토리에서 해주세요
	- 가급적 작업 단위 별로 개인 리포지토리에 작업 내용을 Commit 해주세요.
	- 개인 리포지토리에서도 master(main) 브랜치와 develop 브랜치를 구분해주세요.
		- 지난 작업 복구하기 수월해요
	- 매주 목요일 16:00에 작업 내용 다같이 병합 예정
		- Github Desktop 좌측 Changes에 changed file이 존재한다면 Commit
		- Github Desktop 상단 우측에 Push origin
		- Github.com에서 Contribute -> upstream/graphic_develop에 Merge
		- 모두 merge가 완료되면 소프트 쪽에서 upstream/master에 작업 내용을 모두 병합합니다.
		- 병합이 완료되면 작업 브랜치에서 Sync fork하여 병합된 파일을 받아옵니다.


----- Git LFS 설치 -----
1. https://git-lfs.com/ (Windows 기준) 설치
2. 파일 탐색기에서 프로젝트 폴더 오픈
3. 폴더 내 우클릭하여 Open git bash here 클릭
4. 열린 CMD 창에 git lfs install 입력
5. 	Updated Git hooks.
	Git LFS initialized.
	두 메시지가 출력되면 설치 성공
6. 작업 및 추가한 .uasset, .umap 등의 파일을 github desktop의 change 창에서 확인했을 때, version https://git-lfs.github.com/spec/v1 이런 게 뜨면 성공
	- 만약 특정 파일에 대해 해당 메시지가 안뜨면 소프트 쪽에 바로 연락주세요 
