[Github] 그래픽 작업 시 유의사항


1. Project/Content/Graphic 폴더 외 다른 폴더에서 파일 수정, 생성, 삭제 등의 작업은 삼가주세요.
	- 플러그인 추가 및 프로젝트 설정을 바꿔야할 때 연락주세요
	- 필요한 경우, 소프트 쪽에 바로 연락주세요
	- 특히 소프트 개인 작업 공간에서는 작업을 피해주세요
		- Project/C++ Classes
		- Project/Content/LKH
		- Project/Content/Seohyeon

2. Graphic 폴더 내에서도 가급적이면 개인 폴더를 생성하여, 개인 폴더 내에서만 작업을 진행해주세요.
	- 같은 폴더 내 파일 이름이 겹치거나, 다른 사람이 같은 파일을 작업하면 크러시가 발생해요😭
	- 지속적으로 해당 문제가 발생하면 소프트 쪽에 바로 연락주세요

3. 작업은 개인 브랜치에서 해주세요
	- 작업 시작 전, 꼭 "개인 브랜치인지" / "Fetch origin" 클릭하여 다운받을 내용이 없는지 확인해주세요.
	- 작업 단위 별로 개인 브랜치에 작업 내용을 Commit 해주세요.
		- Github Desktop 좌측 Changes에 changed file이 존재한다면 Commit
	- 매일 작업이 완료되면 Push origin 해주세요.
		- Github Desktop 상단 우측에 Push origin
	- 2~3일 간격으로 소프트 측에서 확인하여 graphic_develop 브랜치에 병합해둘 예정.
		- 추후 관리에 이상이 없으면 그래픽 측 담당자 1분 정해서 해당 작업 진행해줄 것.
	- 매주 목요일 16:00에 소프트 작업 내용 병합 예정
		- 모두 merge가 완료되면 소프트 쪽에서 upstream/master에 작업 내용을 모두 병합합니다.
		- 병합이 완료되면 그래픽 리포지토리의 master 브랜치에 병합된 파일을 받아옵니다. (Github.com -> Sync fork)
		- 병합된 파일에 문제가 없다면 master 브랜치를 개인 브랜치에 Sync fork 합니다.
		 	----- 위 작업까지는 머지 담당자가 해드립니다.
			----- 그 전에 기존 작업 내용을 모두 Push해서 개인 브랜치의 changed file에 아무것도 없도록 해주세요
		- Github Desktop의 개인 브랜치에서 Fetch origin을 클릭하여 변경된 내용을 다운로드 받습니다.	


----- 사전 설치 (Windows 기준) -----
- Unreal 5.4.4
- Github Desktop: https://desktop.github.com/download/
- Git: https://git-scm.com/install/windows
- Git LFS: https://git-lfs.com/
	- 작업 및 추가한 .uasset, .umap 등의 파일을 github desktop의 change 창에서 확인했을 때, version https://git-lfs.github.com/spec/v1 이런 게 뜨면 성공
	- 만약 특정 파일에 대해 해당 메시지가 안뜨면 소프트 쪽에 바로 연락주세요 




참고 사이트
- https://gbleem.tistory.com/84
- https://raindrovvv.tistory.com/29
- https://raindrovvv.tistory.com/30
