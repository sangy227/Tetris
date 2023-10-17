#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Audio.hpp>


using namespace sf;

const int M = 30; //전체 영역 y값
const int N = 20; //전체 영역 x값

int field[M][N] = { 0 };

struct Point
{ int x, y;} a[4], b[4];

int figures[7][4] = {
    1,3,5,7, // 4칸 일자 블럭
    2,4,5,7, // Z블럭
    3,5,4,6, // Z블럭 반전
    3,5,4,7, // ㅓ 블럭
    2,3,5,7, // L 블럭
    3,5,7,6, // L 반전
    2,3,4,5, // ㅁ 블럭

};

bool check() { //20칸 10칸 밖으로 못나가게 하는거
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
        else if (field[a[i].y][a[i].x]) return 0;

    return 1;
}


int main()
{
    sf::Music music;
    if(music.openFromFile("Tetris_Audio.wav"))
        music.play();

    


    srand(time(0));

    RenderWindow window(VideoMode(480, 720), "4 block!"); //해상도 조정

    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/bg.png");
    t3.loadFromFile("images/frame.png");

    Sprite s(t1), background(t2), frame(t3);

    s.setTextureRect(IntRect(0, 0, 18, 18)); // 한개 타일 너비 18px

    int dx = 0; bool rotate = 0; int colorNum = 1;
    float timer = 0, delay = 0.2;

    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed) // x버튼 눌렀을때 종료시키기!
                window.close();

            if (e.type == Event::KeyPressed) //키보드 좌우 입력
                if (e.key.code == Keyboard::Up) rotate = true;
                else if (e.key.code == Keyboard::Left) dx = -1;
                else if (e.key.code == Keyboard::Right) dx = 1;
            
                
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.1;

    //// <- 움직이게 하는 영역 ->////
        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i];
            a[i].x += dx;
        }
        if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

    ////<- 블럭 회전시키게 만드는 곳 ->////
    if (rotate)
    {
        Point p = a[1]; // 가운데 of 순환
        for (int i = 0; i < 4; i++) {
            int x = a[i].y - p.y;
            int y = a[i].x - p.x;
            a[i].x = p.x - x;
            a[i].y = p.y + y;
        }
        if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
    }

    //// <- 타임설정 -> ////
    if (timer > delay) {
        for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }

        if (!check()) {
            for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum; //색깔 지정하기
            colorNum = 1 + rand() % 7;
            int n = rand() % 7;
            for (int i = 0; i < 4; i++) {
                a[i].x = figures[n][i] % 2;
                a[i].y = figures[n][i] / 2;
            }
        }

        timer = 0;
    }

    ////가로 1줄 맞췄을때 없애기////
    int k = M - 1;
    for (int i = M - 1; i > 0; i--) {
        int count = 0;
        for (int j = 0; j < N; j++) {
            if (field[i][j]) count++;
            field[k][j] = field[i][j];
        }
        if (count < N) k--;
    }




    dx = 0; rotate = 0; delay = 0.2;

    //// DRAW 그림 그리는 곳////
    window.clear(Color::White); // 화이트 배경 색 칠하기
    window.draw(background);

    for(int i =0;i<M;i++)
        for (int j = 0; j < N; j++) {
            if (field[i][j] == 0) continue;
            s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
            s.setPosition(j * 18, i * 18);
            s.move(28, 31); //offset
            window.draw(s);
        }

    for (int i = 0; i < 4; i++) {
        s.setTextureRect(IntRect(colorNum* 18, 0, 18, 18));
        s.setPosition(a[i].x * 18, a[i].y * 18);
        s.move(28, 31); //offset
        window.draw(s);
    }
    window.draw(frame);
    
    window.display();//디스플레이 표시
    }

    return 0;
}