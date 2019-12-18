#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "text_input.h"
#include "Timer.h"
using namespace sf;
using namespace std;

void readBallFile(string filename, map<int,float> &xmap, map<int,float> &ymap);
void showImage(RenderWindow &window, Sprite sprite, Texture texture, string imageName,int x,int y);
void readTeamFile(string filename, vector<map<int,float>> &allplayersX, vector<map<int,float>> &allplayersY);

int main(){
    int start = 0;
    int pause = 0;

    float xscale = 255.0f/34.0f;//adjusting coordinate system
    float yscale = 400.0f/52.0f;

    int finalx = 400;
    int finaly = 255;
    //int frames = 7;
    //int playersPerTeam = 14;
    // center is 400 over on columns, 255 down on rows
    //the key of each map will be the time
    //the value of each map will be the position
    map<int,float> ballx; 
    map<int,float> bally;
    vector<map<int,float>> teamOnex;
    vector<map<int,float>> teamOney;
    vector<map<int,float>> teamTwox;
    vector<map<int,float>> teamTwoy;
    
    RectangleShape rectBox(Vector2f(70.0f,30.0f));

    int timeBegin;
    int timeEnd;
    int newStart;
    int newEnd;

    Texture field;
    Texture button;
    Texture ball;
    Texture indiv;
    Texture box;
    Texture forward;

    Sprite background;
    Sprite players;
    Sprite team;
    Sprite textBox;
    Sprite rewind;

    int count;
    Font textFont;
    textFont.loadFromFile("fonts/font.ttf");

    Textbox startInput(20,Color::White,false);
    startInput.setFont(textFont);
    startInput.setPosition({125,530});
    startInput.setLimit(true,5);

    Textbox endInput(20,Color::White,false);
    endInput.setFont(textFont);
    endInput.setPosition({640,530});
    endInput.setLimit(true,5);
    
    Text startText;
    startText.setFont(textFont);
    startText.setString("Start Time -");
    startText.setFillColor(Color::Black);
    startText.setCharacterSize(30);
    startText.setPosition(10,520);
    
    Text endText;
    endText.setFont(textFont);
    endText.setString("- End Time");
    endText.setFillColor(Color::Black);
    endText.setCharacterSize(30);
    endText.setPosition(705,520);

    Text playTime;
    playTime.setFont(textFont);
    playTime.setFillColor(Color::Black);
    playTime.setCharacterSize(30);


    // reading in the files and filling all maps
    readBallFile("files/balldata", ballx,bally);
    readTeamFile("files/team1data", teamOnex,teamOney);
    readTeamFile("files/team2data", teamTwox,teamTwoy);


    RenderWindow window(VideoMode(800, 570), "Visualization");
    Time elapsed;
    Timer watch;
    Event event;
    while (window.isOpen()){
        Vector2i mousePosition = Mouse::getPosition(window);
        int y = mousePosition.x; // over on columns
        int x = mousePosition.y;  // down on rows

        while (window.pollEvent(event)){
			if (event.type == Event::Closed){
				window.close();
            }
            if(event.type == Event::MouseButtonPressed){
                if (event.mouseButton.button == Mouse::Left){
                    if (x >= 515 && x <= 565 ){//lower bar
                        if (y >= 320 && y <= 370){ // play button
                            //start the visulaization
                            cout << "you clicked the play button" << endl;
                            cout << "start time: " << startInput.getText() << endl;
                            cout << "end time: " << endInput.getText() << endl;
                            //Clock clock; // start the clock
                            timeBegin = stoi(startInput.getText());
                            timeEnd = stoi(endInput.getText());
                            //count = timeBegin;
                            if (pause == 1){ // if the game has been paused and you want to continue it
                                pause = 0;
                                watch.resume();
                            }else{
                                watch.toggle(); // starts timer
                                count = timeBegin;
                            }
                            startInput.setSelected(false);
                            endInput.setSelected(false);
                            start = 1;
                        }else if(y >= 430 && y <= 480){ // pause button
                            cout << "you clicked the pause button" << endl;
                            pause = 1;
                            watch.pause();
                            //stop visualization from progrressing
                        }else if(y >= 120 && y <= 190){ // start input box
                            cout << "you clicked the start input button" << endl;
                            startInput.setSelected(true);
                            endInput.setSelected(false);

                        }else if(y >= 630 && y <= 700){ // end input box
                            cout << "you clicked the end input button" << endl;
                            startInput.setSelected(false);
                            endInput.setSelected(true);
                        }else if(y>=495&&y<=530){ // fastforward
                            count += 10;
                        }else if (y>=270&&y<=305){ //reverse
                            count -= 10;  
                        }
                    }
                }
            }
            if(event.type == Event::TextEntered){
                startInput.typedOn(event);
                endInput.typedOn(event);
            }
        }   
        window.clear(Color::White);
        //field
        showImage(window,background,field,"images/field.png",0,0);
        //play button
        showImage(window,background,button,"images/play.png",515,320);
        //pause button
        showImage(window,background,button,"images/pause.png",515,430);
        //fastforward
        showImage(window,rewind,forward,"images/fastforward.png",530,495);
        //reverse
        showImage(window,rewind,forward,"images/reverse.png",530,270);

        //boxes
        rectBox.setFillColor(Color::Black);
        rectBox.setPosition(120,525);//start
        window.draw(rectBox);
        rectBox.setPosition(630,525);//end
        window.draw(rectBox);

        window.draw(startText);
        window.draw(endText);

        endInput.drawTo(window);
        startInput.drawTo(window);

        window.draw(playTime);
        if (start == 1){ //start vis
            if (pause == 0){ // continue game
                if (count < timeEnd){
                    elapsed = watch.getElapsedTime();
                    playTime.setString(to_string(count));
                    if (elapsed.asMilliseconds() % 7 != 0){
                        for(int i = 0; i < 14; i++){
                            //team 1
                            if (teamOnex[i][count] >= 0){
                                finalx = 400 + xscale*teamOnex[i][count];
                            }else if(teamOnex[i][count] < 0){
                                finalx = 400 - xscale*(-1*teamOnex[i][count]);
                            }
                            if (teamOney[i][count] >= 0){
                                finaly = 255 - yscale*teamOney[i][count];
                            }else if (teamOney[i][count] < 0){
                                finaly = 255 + yscale*(-1*teamOney[i][count]);
                            }

                            if (teamOnex[i][count] != -100){//player is a subsitute
                                showImage(window, players,ball,"images/blue.png",finaly,finalx);
                            }
                            //team 2
                            if (teamTwox[i][count] >= 0){
                                finalx = 400 + xscale*teamTwox[i][count];
                            }else if (teamTwox[i][count] < 0){
                                finalx = 400 - xscale*(-1*teamTwox[i][count]);
                            }
                            if (teamTwoy[i][count] >= 0){
                                finaly = 255 - yscale*teamTwoy[i][count];
                            }else if (teamTwoy[i][count] < 0){
                                finaly = 255 + yscale*(-1*teamTwoy[i][count]);
                            }

                            if (teamTwox[i][count] != -100){//player is not a subsitute
                                showImage(window, players,ball,"images/red.png",finaly,finalx);
                            }

                        }


                        if (ballx[count] >= 0){
                            finalx = 400 + xscale*ballx[count];
                        }else if (ballx[count] < 0){
                            finalx = 400 - xscale*(-1*ballx[count]);
                        }
                        if (bally[count] >= 0){
                            finaly = 255 - yscale*bally[count];
                        }else if (bally[count] < 0){
                            finaly = 255 + yscale*(-1*bally[count]);
                        }
                        
                        cout << "over on col: " << finaly << endl;
                        cout << "down on row: " << finalx << endl;

                        // if (finalx < 0 || finalx > 800 || finaly < 0 || finaly > 510){
                        //     //this will show ball going outside of frame
                        // }else{
                        //     showImage(window, players,ball,"images/ball.png",finaly,finalx);
                        // }
                        showImage(window, players,ball,"images/ball.png",finaly,finalx);
                    }else{
                        count++;
                    }
                }
            }else{ //game is paused, stop everything where it is
                if (ballx[count] >= 0){
                    finalx = 400 + xscale*ballx[count];
                }else if (ballx[count] < 0){
                    finalx = 400 - xscale*(-1*ballx[count]);
                }
                if (bally[count] >= 0){
                    finaly = 255 - yscale*bally[count];
                }else if (bally[count] < 0){
                    finaly = 255 + yscale*(-1*bally[count]);
                }
                showImage(window, players,ball,"images/ball.png",finaly,finalx);
                playTime.setString(to_string(count));
                for(int i = 0; i < 14; i++){
                    if (teamOnex[i][count] >= 0){
                        finalx = 400 + xscale*teamOnex[i][count];
                    }else if(teamOnex[i][count] < 0){
                        finalx = 400 - xscale*(-1*teamOnex[i][count]);
                    }
                    if (teamOney[i][count] >= 0){
                        finaly = 255 - yscale*teamOney[i][count];
                    }else if (teamOney[i][count] < 0){
                        finaly = 255 + yscale*(-1*teamOney[i][count]);
                    }

                    if (teamOnex[i][count] != -100){//player is a subsitute
                        showImage(window, players,ball,"images/blue.png",finaly,finalx);
                    }
                    //team 2
                    if (teamTwox[i][count] >= 0){
                        finalx = 400 + xscale*teamTwox[i][count];
                    }else if (teamTwox[i][count] < 0){
                        finalx = 400 - xscale*(-1*teamTwox[i][count]);
                    }
                    if (teamTwoy[i][count] >= 0){
                        finaly = 255 - yscale*teamTwoy[i][count];
                    }else if (teamTwoy[i][count] < 0){
                        finaly = 255 + yscale*(-1*teamTwoy[i][count]);
                    }

                    if (teamTwox[i][count] != -100){//player is not a subsitute
                        showImage(window, players,ball,"images/red.png",finaly,finalx);
                    }

                }

            }
            if(count < 100){
                playTime.setPosition(390,520);
            }
            else if(count < 1000){
                playTime.setPosition(387,520);

            }else if(count < 10000){
                playTime.setPosition(380,520);

            }else{
                playTime.setPosition(376,520);
            }
            window.draw(playTime);
        }else{ // maybe show starting formation

        }
        window.display();
    }
    return 0;
}

void readBallFile(string filename, map<int,float> &xmap, map<int,float> &ymap){
    ifstream dataFile(filename);
    string line;
    int ballPos;
    float xPos;
    float yPos;
    string token;

    while(getline(dataFile,line)){
        istringstream stream(line);
        getline(stream,token,','); // token is now ball pos
        ballPos = stoi(token);
        getline(stream,token,','); // token is now ball pos
        xPos = stof(token);
        getline(stream,token,','); // token is now ball pos
        yPos = stof(token);

        xmap.emplace(ballPos,xPos);
        ymap.emplace(ballPos,yPos);

    }
    dataFile.close();
}
void showImage(RenderWindow &window, Sprite sprite, Texture texture, string imageName, int x, int y){
    texture.loadFromFile(imageName);
    sprite.setTexture(texture);
    sprite.setPosition(y,x);
    window.draw(sprite);
}
void readTeamFile(string filename, vector<map<int,float>> &allplayersX, vector<map<int,float>> &allplayersY){
    ifstream dataFile(filename);
    string line;
    int time;
    float xPos;
    float yPos;
    string token;
    map<int,float> player1x;
    map<int,float> player1y;
    map<int,float> player2x;
    map<int,float> player2y;
    map<int,float> player3x;
    map<int,float> player3y;
    map<int,float> player4x;
    map<int,float> player4y;
    map<int,float> player5x;
    map<int,float> player5y;
    map<int,float> player6x;
    map<int,float> player6y;
    map<int,float> player7x;
    map<int,float> player7y;
    map<int,float> player8x;
    map<int,float> player8y;
    map<int,float> player9x;
    map<int,float> player9y;
    map<int,float> player10x;
    map<int,float> player10y;
    map<int,float> player11x;
    map<int,float> player11y;
    map<int,float> player12x;
    map<int,float> player12y;
    map<int,float> player13x;
    map<int,float> player13y;
    map<int,float> player14x;
    map<int,float> player14y;

    while(getline(dataFile,line)){
        istringstream stream(line);
        getline(stream,token,','); // token is now ball pos
        time = stoi(token);
        getline(stream,token,','); // token is now ball pos
        xPos = stof(token);
        getline(stream,token,','); // token is now ball pos
        yPos = stof(token);
        player1x.emplace(time,xPos);
        player1y.emplace(time,yPos);

        getline(stream,token,','); // token is now ball pos
        xPos = stof(token);
        getline(stream,token,','); // token is now ball pos
        yPos = stof(token);
        player2x.emplace(time,xPos);
        player2y.emplace(time,yPos);

        getline(stream,token,','); // token is now ball pos
        xPos = stof(token);
        getline(stream,token,','); // token is now ball pos
        yPos = stof(token);
        player3x.emplace(time,xPos);
        player3y.emplace(time,yPos);

        getline(stream,token,','); // token is now ball pos
        xPos = stof(token);
        getline(stream,token,','); // token is now ball pos
        yPos = stof(token);
        player4x.emplace(time,xPos);
        player4y.emplace(time,yPos);

        getline(stream,token,','); // token is now ball pos
        xPos = stof(token);
        getline(stream,token,','); // token is now ball pos
        yPos = stof(token);
        player5x.emplace(time,xPos);
        player5y.emplace(time,yPos);

        getline(stream,token,','); // token is now ball pos
        xPos = stof(token);
        getline(stream,token,','); // token is now ball pos
        yPos = stof(token);
        player6x.emplace(time,xPos);
        player6y.emplace(time,yPos);

        getline(stream,token,','); // token is now ball pos
        xPos = stof(token);
        getline(stream,token,','); // token is now ball pos
        yPos = stof(token);
        player7x.emplace(time,xPos);
        player7y.emplace(time,yPos);

        getline(stream,token,','); // token is now ball pos
        xPos = stof(token);
        getline(stream,token,','); // token is now ball pos
        yPos = stof(token);
        player8x.emplace(time,xPos);
        player8y.emplace(time,yPos);

        getline(stream,token,','); // token is now ball pos
        xPos = stof(token);
        getline(stream,token,','); // token is now ball pos
        yPos = stof(token);
        player9x.emplace(time,xPos);
        player9y.emplace(time,yPos);

        getline(stream,token,','); // token is now ball pos
        xPos = stof(token);
        getline(stream,token,','); // token is now ball pos
        yPos = stof(token);
        player10x.emplace(time,xPos);
        player10y.emplace(time,yPos);

        getline(stream,token,','); // token is now ball pos
        xPos = stof(token);
        getline(stream,token,','); // token is now ball pos
        yPos = stof(token);
        player11x.emplace(time,xPos);
        player11y.emplace(time,yPos);

        getline(stream,token,','); // token is now ball pos
        xPos = stof(token);
        getline(stream,token,','); // token is now ball pos
        yPos = stof(token);
        player12x.emplace(time,xPos);
        player12y.emplace(time,yPos);

        getline(stream,token,','); // token is now ball pos
        xPos = stof(token);
        getline(stream,token,','); // token is now ball pos
        yPos = stof(token);
        player13x.emplace(time,xPos);
        player13y.emplace(time,yPos);

        getline(stream,token,','); // token is now ball pos
        xPos = stof(token);
        getline(stream,token,','); // token is now ball pos
        yPos = stof(token);
        player14x.emplace(time,xPos);
        player14y.emplace(time,yPos);
    }
    allplayersX.push_back(player1x);
    allplayersX.push_back(player2x);
    allplayersX.push_back(player3x);
    allplayersX.push_back(player4x);
    allplayersX.push_back(player5x);
    allplayersX.push_back(player6x);
    allplayersX.push_back(player7x);
    allplayersX.push_back(player8x);
    allplayersX.push_back(player9x);
    allplayersX.push_back(player10x);
    allplayersX.push_back(player11x);
    allplayersX.push_back(player12x);
    allplayersX.push_back(player13x);
    allplayersX.push_back(player14x);

    allplayersY.push_back(player1y);
    allplayersY.push_back(player2y);
    allplayersY.push_back(player3y);
    allplayersY.push_back(player4y);
    allplayersY.push_back(player5y);
    allplayersY.push_back(player6y);
    allplayersY.push_back(player7y);
    allplayersY.push_back(player8y);
    allplayersY.push_back(player9y);
    allplayersY.push_back(player10y);
    allplayersY.push_back(player11y);
    allplayersY.push_back(player12y);
    allplayersY.push_back(player13y);
    allplayersY.push_back(player14y);

    dataFile.close();
}
