#include <SFML/Graphics.hpp>

float speed;
int Screen_W;
int Screen_H;

float scale_it(float q , float a , float b , float c ,float d);
void draw(sf::RenderWindow &app ,sf::CircleShape &circle);
void screenshot(sf::RenderWindow &app);

int count = 0;

class Star{
    float x , y , z , pz;
    sf::Color star_color;

    public:
        Star( )
        {
            x = -Screen_W/2 + rand()% (Screen_W);
            y = -Screen_H/2 + rand()% (Screen_H);
            z = rand() % Screen_W;
            pz = z;
            star_color = sf::Color( rand( ) % 255 , rand( ) % 255 ,rand( ) % 255 );
        }
        void update(){
            z = z - speed;

            if(z<1)
            {
                z = Screen_W;
                x = -Screen_W/2 + rand()% (Screen_W);
                y = -Screen_H/2 + rand()% (Screen_H);
                pz = z;
            }
        }
        void show(sf::RenderWindow &app , sf::CircleShape &circle){

            float sx = scale_it(x/z,0,1,0,Screen_W);
            float sy = scale_it(y/z,0,1,0,Screen_W);
            float r  = scale_it(z , 0 , Screen_W , 8 , 0);


            circle.setPosition(sf::Vector2f(sx,sy));
            circle.setRadius(r);
            circle.setFillColor(star_color);
            app.draw(circle);

            float px = scale_it(x/pz,0,1,0,Screen_W);
            float py = scale_it(y/pz,0,1,0,Screen_W);

            pz = z;

            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(px + r,py + r)),
                sf::Vertex(sf::Vector2f(sx + r,sy + r))
            };

            app.draw(line,2,sf::Lines);

        }
};

std::vector< Star* > stars;
int STAR_COUNT = 500;


int main()
{
    srand(time(NULL));
    sf::RenderWindow app(sf::VideoMode::getFullscreenModes()[0],"Starfield",sf::Style::Fullscreen);

    Screen_W = app.getSize( ).x;
    Screen_H = app.getSize( ).y;   
    
    sf::View view(sf::FloatRect(-Screen_W/2,-Screen_H/2,Screen_W,Screen_H));
    app.setView(view);

    sf::CircleShape circle(4);

    for(int i = 0 ; i < STAR_COUNT ; i++){
        Star *temp = new Star();
        stars.push_back(temp);
    }

    while(app.isOpen())
    {
        sf::Event event;
        while(app.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                app.close( );
                break;

            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Escape:
                    app.close( );
                    break;
                case sf::Keyboard::P:
                    screenshot( app );
                    break;
                default:
                    break;
                }
                break;
            
            default:
                break;
            }
        }
        app.clear();
        speed = scale_it(sf::Mouse::getPosition(app).x , 0 , Screen_W , 0.1 , 50);
        draw(app,circle);
        app.display();
    }
}

void draw(sf::RenderWindow &app , sf::CircleShape &circle)
{
    for(int i = 0 ; i < STAR_COUNT ; i++){
        stars[i]->update();
        stars[i]->show(app , circle);
    }
}

float scale_it(float q , float a , float b , float c ,float d){
    float temp = (b - (q/(b-a)*b))/b;

    return (d- temp*(d-c));
}
void screenshot(sf::RenderWindow &app){
    sf::Texture _tex;
    _tex.create( app.getSize( ).x , app.getSize( ).y );
    _tex.update( app );
    _tex.copyToImage( ).saveToFile("Background"+std::to_string(count)+".png");
    count++;
}