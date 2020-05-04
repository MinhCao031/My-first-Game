# My First Game: Dodge The Bullets

Link download các file include, libraries, ...

https://www.libsdl.org/release/SDL2-devel-2.0.12-mingw.tar.gz (i686-w64-mingw32)
https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.5-mingw.tar.gz
https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.15-mingw.tar.gz
https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.4-mingw.tar.gz

File Debug bao gồm TTF, ảnh, âm nhạc và file .exe

Nội dung game: Người chơi sẽ điều khiển chấm vàng theo các hướng để tránh va chạm với các chấm tròn màu đỏ (địch/chướng ngại/viên đạn/whatever...) bằng cách dùng phím mũi tên, hoặc các phím q,z,c,e trong 1 số cửa nhất định

Sau khi chọn chế độ chơi (có 3 chế độ chơi) và độ khó (có 4 hoặc 8 độ khó, tùy chế độ) người chơi có thể chọn 1 trong 3 mục tiêu để hoàn thành (mục tiêu thứ 3 trong đó ko hẳn là mục tiêu, vì đó là chơi đến khi nào thua hoặc người chơi tự thoát ra ngoài :v)

Khi 1 chấm tròn đỏ đi ra ngoài khỏi cửa sổ trò chơi, người chơi sẽ được nhận 1-số-cố-định điểm (tùy ván chơi)

Khi mục tiêu được hoàn thành (hoặc người chơi để xảy ra va chạm) ngay lập tức game kết thúc, 1 thông báo hiện ra thông báo điểm số cuối cùng và cho người chơi 2 lựa chọn (chơi lại chính cửa đó hoặc thoát ra ngoài)

