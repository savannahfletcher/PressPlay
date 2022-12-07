#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>
#include "Games.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
using namespace std::chrono;
using namespace std;

int calculateFontSize(string favGame) {
    if (favGame.length() < 18) {
        return 100;
    }
    else if (favGame.length() < 35){
        return 80;
    }
    else {
        return 50;
    }
}

int setWidthPosition(string favGame, int fontSize) {
    int pixelsWide = (int)favGame.length() * (fontSize/2);
    return (800 - (pixelsWide/2));
}

string roundPrices(string price) {
    string rounded;
    int decimalIndex = 0;
    for (int i = 0; i < price.length(); i++) {
        if (price[i] == '.') {
            decimalIndex = i;
        }
    }
    rounded = price.substr(0,decimalIndex+3);
    return rounded;
}

string insertNewline(string game, int start) {
    // given string length is 40 characters or more
    for (int i = start; i < game.length(); i++) {
        if (game[i] == ' ') {
            game.insert(i, "\n");
            break;
        }
    }
    return game;
}

vector<string> getUserIDs(){
    vector<string> userIDs;
    userIDs.push_back("76561198264290642"); // savannah's ID
    userIDs.push_back("76561198273516178"); // maddie's ID
    userIDs.push_back("22222222222222222");
    for (int i = 0; i < 89997; i++){
        string ID = "";
        for (int j = 0; j < 17; j++){
            int random = rand()%10;
            ID+= to_string(random);
        }
        userIDs.push_back(ID);
    }
    return userIDs;
}

vector<Game*> getGames(ifstream& file, string fileName){

    string n, r, m, c, f, g, i, ac, ad, ca, s, rpg, sim, ea, ftp, spo, rac, mmp, price;
    string genre;
    double pr;

    vector<Game*> games;
    fileName = resourcePath() + "games-features-edit.csv";

    file.open(fileName);
    string line;
    getline(file, line);

    while (getline(file, line)) {
        istringstream stream(line);

        getline(stream, n, ',');
        getline(stream, r, ',');
        getline(stream, m, ',');
        getline(stream, c, ',');
        getline(stream, f, ',');
        getline(stream, g, ',');
        getline(stream, i, ',');
        getline(stream, ac, ',');
        getline(stream, ad, ',');
        getline(stream, ca, ',');
        getline(stream, s, ',');
        getline(stream, rpg, ',');
        getline(stream, sim, ',');
        getline(stream, ea, ',');
        getline(stream, ftp, ',');
        getline(stream, spo, ',');
        getline(stream, rac, ',');
        getline(stream, mmp, ',');
        getline(stream, price, ',');

        n.erase(remove(n.begin(), n.end(), '"'), n.end());

        if (g == "TRUE") {
            genre = "Non Game";
        }
        else if (i == "TRUE") {
            genre = "Indie";
        }
        else if (ac == "TRUE") {
            genre = "Action";
        }
        else if (ad == "TRUE") {
            genre = "Adventure";
        }
        else if (ca == "TRUE") {
            genre = "Casual";
        }
        else if (s == "TRUE") {
            genre = "Strategy";
        }
        else if (rpg == "TRUE") {
            genre = "RPG";
        }
        else if (sim == "TRUE") {
            genre = "Simulation";
        }
        else if (ea == "TRUE") {
            genre = "Early Access";
        }
        else if (ftp == "TRUE") {
            genre = "Free to Play";
        }
        else if (spo == "TRUE") {
            genre = "Sports";
        }
        else if (rac == "TRUE") {
            genre = "Racing";
        }
        else if (mmp == "TRUE") {
            genre = "Massively Multiplayer";
        }

        pr = ceil(stod(price));

        Game* game = new Game(n, genre, pr);
        games.push_back(game);
    }
    return games;
}

map<string,Game*> peopleWithGames(vector<string>& userIDs, vector<Game*>& games){
    map<string,Game*> peopleGame;

    
    Game* savannah;
    savannah = games[2913];
    peopleGame["76561198264290642"] = savannah;
    Game* maddie;
    maddie = games[8086];
    peopleGame["76561198273516178"] = maddie;
    for (int i = 2; i < userIDs.size(); i++){
        int random = rand()%12625;
        Game* game = games[random];
        peopleGame[userIDs[i]] = game;
    }
    return peopleGame;
}

map<string,vector<Game*>> createGenreGraph(vector<Game*>& games){
    map<string,vector<Game*>> genreMap;

    for (int i = 0; i < games.size(); i++){
        genreMap[games[i]->getGenre()].push_back(games[i]);
    }
    return genreMap;
}

map<double,vector<Game*>> createPriceGraph(vector<Game*>& games){
    map<double,vector<Game*>> priceMap;

    for (int i = 0; i < games.size(); i++){
        priceMap[games[i]->getPrice()].push_back(games[i]);
    }
    return priceMap;
}

map<string,string> createGenreMap(vector<Game*>& games){
    map<string,string> genreMap;

    for (int i = 0; i < games.size(); i++){
        genreMap[games[i]->getName()] = games[i]->getGenre();
    }
    return genreMap;
}

map<string,double> createPriceMap(vector<Game*>& games){
    map<string, double> priceMap;

    for (int i = 0; i < games.size(); i++){
        priceMap[games[i]->getName()] = games[i]->getPrice();
    }
    return priceMap;
}


vector<string> searchMap (map<string,string> genreMap, map<string,double> priceMap, string userID, map<string,Game*> peopleGame){
    Game* game = peopleGame[userID];
    vector<string> res;

    for (auto itr = genreMap.begin(); itr != genreMap.end(); itr++){
        if (itr->second == game->getGenre()){
            res.push_back(itr->first);
        }
    }
    for (auto itr = priceMap.begin(); itr != priceMap.end(); itr++){
        if (itr->second == game->getPrice()){
            res.push_back(itr->first);
        }
    }
    sort(res.begin(), res.end());
    res.erase(unique(res.begin(), res.end()), res.end());
    return res;
}

vector<string> searchGraph(map<string,vector<Game*>> genreGraph, map<double,vector<Game*>> priceGraph, string userID, map<string,Game*> peopleGame){
    Game* game = peopleGame[userID];
    vector<string> res;

    for (int i = 0; i < genreGraph[game->getGenre()].size(); i++){
        res.push_back(genreGraph[game->getGenre()][i]->getName());
    }
    for (int i = 0; i < priceGraph[game->getPrice()].size(); i++){
        res.push_back(priceGraph[game->getPrice()][i]->getName());
    }
    sort(res.begin(), res.end());
    res.erase(unique(res.begin(), res.end()), res.end());
    return res;
}

string findGenre(string game, vector<Game*> games){
    string res;
    for (int i = 0; i < games.size(); i++){
        if (games[i]->getName() == game){
            res = games[i]->getGenre();
            break;
        }
    }
    return res;
}
double findPrice(string game, vector<Game*> games){
    double res = 0;
    for (int i = 0; i < games.size(); i++){
        if (games[i]->getName() == game){
            res = games[i]->getPrice();
            break;
        }
    }
    return res;
}


int main() {
    
    srand(time(NULL));

        ifstream file;
        string fileName;

        vector<string> userIDs = getUserIDs(); //vector of random user IDS

        vector<Game*> games = getGames(file, fileName); //vector of game objects -> genre, price, name

        map<string,Game*> peopleGame = peopleWithGames(userIDs, games); //map of key: userIDS, value: their game

        //graph implementation
        map<string,vector<Game*>> genreGraph = createGenreGraph(games); //key: genre, value: games with that genre

        map<double,vector<Game*>> priceGraph = createPriceGraph(games);//key: price, value: games with that price

        //map implementation
        map<string,string> genreMap = createGenreMap(games); //key: game name, value: genre of that game
    
        map<string,double> priceMap = createPriceMap(games); //key: game name, value: price of that game
    

    
    
    int screenNum = 1;
    int numDigits = 0;
    int mapOrGraph = 0;
    bool mapHover = false;
    bool graphHover = false;
    vector<string> recGames;
    vector<string> times;
    // Creates the main window
    sf::RenderWindow window(sf::VideoMode(1500, 1000), "PressPlay.exe");
    
    // Screen 1 text
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "Computerfont.ttf")) {
        cout << "Error loading text" << endl;
        return EXIT_FAILURE;
    }
    sf::Text text;
    text.setFont(font);
    text.setString("Welcome, player!");
    text.setCharacterSize(100);
    text.setFillColor(sf::Color::White);
    sf::Text text2;
    text2.setFont(font);
    text2.setString("Input Steam ID:");
    text2.setCharacterSize(70);
    text2.setFillColor(sf::Color::White);
    // intaking user input for player's Steam ID
    sf::String playerInput;
    sf::Text playerText;
    playerText.setFont(font);
    playerText.setCharacterSize(70);
    playerText.setFillColor(sf::Color::White);
    
    // Screen 2 text
    sf::Text text3;
    text3.setFont(font);
    text3.setString("Looks like your favorite game on Steam is: ");
    text3.setCharacterSize(70);
    text3.setFillColor(sf::Color::White);
    sf::Text text5;
    text5.setFont(font);
    text5.setString("Select data structure: ");
    text5.setCharacterSize(60);
    text5.setFillColor(sf::Color::White);
    sf::Text Map;
    Map.setFont(font);
    Map.setString("Map");
    Map.setCharacterSize(60);
    Map.setFillColor(sf::Color::White);
    sf::Text Graph;
    Graph.setFont(font);
    Graph.setString("Graph");
    Graph.setCharacterSize(60);
    Graph.setFillColor(sf::Color::White);
    
    // Screen 3 text
    sf::Text loading;
    loading.setFont(font);
    loading.setString("Loading your recommendations...");
    loading.setCharacterSize(100);
    loading.setFillColor(sf::Color::White);
    
    // Screen 4 text
    sf::Text text4;
    text4.setFont(font);
    text4.setString("Based on this game, we think you might also like...");
    text4.setCharacterSize(60);
    text4.setFillColor(sf::Color::White);
    sf::Text game;
    game.setFont(font);
    game.setString("Game");
    game.setCharacterSize(60);
    game.setFillColor(sf::Color::White);
    sf::Text genre;
    genre.setFont(font);
    genre.setString("Genre");
    genre.setCharacterSize(60);
    genre.setFillColor(sf::Color::White);
    sf::Text price;
    price.setFont(font);
    price.setString("Price");
    price.setCharacterSize(60);
    price.setFillColor(sf::Color::White);
    
    // Screen 1 Shapes
    sf::RectangleShape rectangle(sf::Vector2f(800, 100));
    rectangle.setOutlineColor(sf::Color::White);
    sf::RectangleShape rectangle2(sf::Vector2f(790, 90));
    rectangle2.setFillColor(sf::Color::Black);
    sf::RectangleShape underline(sf::Vector2f(475, 5));
    underline.setOutlineColor(sf::Color::White);
    // loading heart sprite
    sf::Texture heartT;
    heartT.loadFromFile(resourcePath() + "heart.png");
    sf::Sprite heart(heartT);
    // loading play button sprite
    sf::Texture playT;
    playT.loadFromFile(resourcePath() + "playbutton.png");
    sf::Sprite playButton(playT);
    
    // Screen 2 Shapes
    sf::RectangleShape underline3(sf::Vector2f(100, 5));
    underline3.setOutlineColor(sf::Color::White);
    sf::RectangleShape underline4(sf::Vector2f(140, 5));
    underline4.setOutlineColor(sf::Color::White);
    sf::RectangleShape underline5(sf::Vector2f(1270, 5));
    underline5.setOutlineColor(sf::Color::White);
    
    // Screen 4 Shapes
    sf::RectangleShape underline2(sf::Vector2f(1225, 5));
    underline2.setOutlineColor(sf::Color::White);
   

    // Start the game loop
    while (window.isOpen())
    {
        sf::Vector2i clickPos = sf::Mouse::getPosition(window);
        int x = clickPos.x;
        int y = clickPos.y;
        
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            // Intaking text for player's Steam ID
            if (event.type == sf::Event::TextEntered && numDigits < 17 && event.text.unicode != '\b') {
                playerInput += event.text.unicode;
                playerText.setString(playerInput);
                numDigits++;
            }
            if (event.type == sf::Event::TextEntered && event.text.unicode == '\b') {
                playerInput.erase(playerInput.getSize()-1, 1);
                playerText.setString(playerInput);
                numDigits--;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                // if mouse is left-clicked on first screen
                if (screenNum == 1 && event.mouseButton.button == sf::Mouse::Left) {
                    //if play button is clicked
                    if (x > 560 && y > 625 && x < 825 && y < 800 && numDigits == 17) {
                        screenNum++;
                    }
                }
                //if mouse left-clicked on 2nd screen
                if (screenNum == 2 && event.mouseButton.button == sf::Mouse::Left) {
                    if (x > 900 && y > 800 && x < 1000 && y < 960) {
                        mapOrGraph = 1;
                    }
                    //if within bounds of graph button: mapOrGraph = 2
                    if (x > 1100 && y > 800 && x < 1290 && y < 860) {
                        mapOrGraph = 2;
                    }
                }
            }
            //if within bounds of map button: mapHover = true
            if (x > 900 && y > 800 && x < 1000 && y < 960) {
                mapHover = true;
            }
            //if within bounds of graph button: graphHover = true
            else if (x > 1100 && y > 800 && x < 1290 && y < 860) {
                graphHover = true;
            }
            else {
                mapHover = false;
                graphHover = false;
            }
        }

        // Update the output
        window.clear();
        
        //initial screen to input player's Steam ID
        if (screenNum == 1) {
            //draw text
            text.setPosition(385, 100);
            window.draw(text);
            text2.setPosition(510, 300);
            window.draw(text2);
            underline.setPosition(500, 395);
            window.draw(underline);
            //draw hearts
            heart.setPosition(175, 250);
            window.draw(heart);
            heart.setPosition(1250, 250);
            window.draw(heart);
            // input box
            rectangle.setPosition(350,500);
            window.draw(rectangle);
            rectangle2.setPosition(355, 505);
            window.draw(rectangle2);
            // play button box
            playButton.setPosition(560, 600);
            window.draw(playButton);
            
            //drawing player's input while key strokes detected
            playerText.setPosition(420, 500);
            window.draw(playerText);
        }
        // displaying favorite game screen
        else if (screenNum == 2) {
            
            //call function with playerInputer (player ID) to get their favorite game (string)
            string favGame = peopleGame[playerInput]->getName();
            
            //get vector of recommended games via map
            auto start = high_resolution_clock::now();
            vector<string> mapRecommend = searchMap(genreMap, priceMap, playerInput, peopleGame);
            auto stop = high_resolution_clock::now();

            auto durationMap = duration_cast<microseconds>(stop - start);
            string duration = to_string((int)durationMap.count());
            times.push_back(duration); // map run time stored at index 0
        
            //get vector of recommended games via graph
            start = high_resolution_clock::now();
            vector<string> graphRecommend = searchGraph(genreGraph, priceGraph, playerInput, peopleGame);
            stop = high_resolution_clock::now();

            auto durationGraph = duration_cast<microseconds>(stop - start);
            duration = to_string((int)durationGraph.count());
            times.push_back(duration); // graph run time stored at index 1

            int size = 0;
            if (mapRecommend.size() < graphRecommend.size()){
                size = (int)mapRecommend.size();
            }
            else {
                size = (int)graphRecommend.size();
            }

            int game1Rand = rand()%size; //random index from vector
            int game2Rand = rand()%size;
            int game3Rand  = rand()%size;

            if (mapOrGraph == 1) {
                string game1Map = mapRecommend[game1Rand]; //game 1 name
                recGames.push_back(game1Map);
                string game1GenreM = findGenre(game1Map, games); //respective genre
                recGames.push_back(game1GenreM);
                string game1PriceM = "$" + to_string(findPrice(game1Map, games)); //respective price
                recGames.push_back(game1PriceM);
                
                string game2Map = mapRecommend[game2Rand];
                recGames.push_back(game2Map);
                string game2GenreM = findGenre(game2Map, games);
                recGames.push_back(game2GenreM);
                string game2PriceM = "$" + to_string(findPrice(game2Map, games));
                recGames.push_back(game2PriceM);
                
                string game3Map = mapRecommend[game3Rand];
                recGames.push_back(game3Map);
                string game3GenreM = findGenre(game3Map, games);
                recGames.push_back(game3GenreM);
                string game3PriceM = "$" + to_string(findPrice(game3Map, games));
                recGames.push_back(game3PriceM);
                screenNum++;
            }
            else if (mapOrGraph == 2) {

                string game1Graph = graphRecommend[game1Rand]; //same sitch
                recGames.push_back(game1Graph);
                string game1GenreG = findGenre(game1Graph, games);
                recGames.push_back(game1GenreG);
                string game1PriceG = "$" + to_string(findPrice(game1Graph, games));
                recGames.push_back(game1PriceG);

                string game2Graph = graphRecommend[game2Rand];
                recGames.push_back(game2Graph);
                string game2GenreG = findGenre(game2Graph, games);
                recGames.push_back(game2GenreG);
                string game2PriceG = "$" + to_string(findPrice(game2Graph, games));
                recGames.push_back(game2PriceG);

                string game3Graph = graphRecommend[game3Rand];
                recGames.push_back(game3Graph);
                string game3GenreG = findGenre(game3Graph, games);
                recGames.push_back(game3GenreG);
                string game3PriceG = "$" + to_string(findPrice(game3Graph, games));
                recGames.push_back(game3PriceG);
                screenNum++;
            }

            sf::Text fav;
            fav.setFont(font);
            fav.setString(favGame);
            if (favGame.length() >= 50) {
                string game = insertNewline(favGame, 40);
//                if (favGame.length() > 60) {
//                    game = insertNewline(favGame, );
//                }
                fav.setString(game);
            }
            int fontSize = calculateFontSize(favGame);
            fav.setCharacterSize(fontSize);
            fav.setFillColor(sf::Color::White);
            
            text3.setPosition(100, 200);
            window.draw(text3);
            underline5.setPosition(100, 300);
            window.draw(underline5);
            fav.setPosition(setWidthPosition(favGame, fontSize), 350);
            window.draw(fav);
            text5.setPosition(250, 800);
            window.draw(text5);
            Map.setPosition(900, 800);
            window.draw(Map);
            Graph.setPosition(1100, 800);
            window.draw(Graph);
            if (mapHover == true) {
                underline3.setPosition(900, 875);
                window.draw(underline3);
            }
            else if (graphHover == true) {
                underline4.setPosition(1100, 875);
                window.draw(underline4);
            }
        }
        // loading results screen
        else if (screenNum == 3) {
            sf::Clock freezeClock;
            while (freezeClock.getElapsedTime() < sf::seconds(5.f)) {
                window.clear();
                loading.setPosition(95, 400);
                window.draw(loading);
                window.display();
            }
            window.clear();
            screenNum++;
        }
        
        // displaying table of game recommendations
        else if (screenNum == 4) {
            text4.setPosition(100, 50);
            window.draw(text4);
            game.setPosition(150, 200);
            window.draw(game);
            genre.setPosition(850, 200);
            window.draw(genre);
            price.setPosition(1200, 200);
            window.draw(price);
            underline2.setPosition(125, 275);
            window.draw(underline2);
            
            //use recGame vector to print the recommended games along with their genres & prices
            sf::Text game1;
            game1.setFont(font);
            game1.setString(recGames[0]);
            if (recGames[0].length() < 25) {
                game1.setCharacterSize(50);
            }
            else if (recGames[0].length() < 40) {
                game1.setCharacterSize(35);
            }
            else {
                //need to insert a newling to fit string
                string game = insertNewline(recGames[0], 25);
                if (recGames[0].length() > 60) {
                    game = insertNewline(game, 50);
                }
                game1.setString(game);
                game1.setCharacterSize(35);
            }
            game1.setFillColor(sf::Color::White);
            game1.setPosition(150, 350);
            window.draw(game1);
            sf::Text game2;
            game2.setFont(font);
            game2.setString(recGames[3]);
            if (recGames[3].length() < 25) {
                game2.setCharacterSize(50);
            }
            else if (recGames[3].length() < 40) {
                game2.setCharacterSize(35);
            }
            else {
                string game = insertNewline(recGames[3], 25);
                if (recGames[3].length() > 60) {
                    game = insertNewline(game, 50);
                }
                game2.setString(game);
                game2.setCharacterSize(35);
            }
            game2.setFillColor(sf::Color::White);
            game2.setPosition(150, 550);
            window.draw(game2);
            sf::Text game3;
            game3.setFont(font);
            game3.setString(recGames[6]);
            if (recGames[6].length() < 25) {
                game3.setCharacterSize(50);
            }
            else if (recGames[6].length() < 40) {
                game3.setCharacterSize(35);
            }
            else {
                string game = insertNewline(recGames[6], 25);
                if (recGames[6].length() > 60) {
                    game = insertNewline(game, 50);
                }
                game3.setString(game);
                game3.setCharacterSize(35);
            }
            game3.setFillColor(sf::Color::White);
            game3.setPosition(150, 750);
            window.draw(game3);
            
            //printing genres
            sf::Text genre1;
            genre1.setFont(font);
            genre1.setString(recGames[1]);
            if (recGames[1].length() < 25) {
                genre1.setCharacterSize(50);
            }
            else {
                genre1.setCharacterSize(35);
            }
            genre1.setFillColor(sf::Color::White);
            genre1.setPosition(850, 350);
            window.draw(genre1);
            sf::Text genre2;
            genre2.setFont(font);
            genre2.setString(recGames[4]);
            if (recGames[4].length() < 25) {
                genre2.setCharacterSize(50);
            }
            else {
                genre2.setCharacterSize(35);
            }
            genre2.setFillColor(sf::Color::White);
            genre2.setPosition(850, 550);
            window.draw(genre2);
            sf::Text genre3;
            genre3.setFont(font);
            genre3.setString(recGames[7]);
            if (recGames[7].length() < 25) {
                genre3.setCharacterSize(50);
            }
            else {
                genre3.setCharacterSize(35);
            }
            genre3.setFillColor(sf::Color::White);
            genre3.setPosition(850, 750);
            window.draw(genre3);
            
            //printing prices
            sf::Text price1;
            price1.setFont(font);
            price1.setString(roundPrices(recGames[2]));
            price1.setCharacterSize(50);
            price1.setFillColor(sf::Color::White);
            price1.setPosition(1200, 350);
            window.draw(price1);
            sf::Text price2;
            price2.setFont(font);
            price2.setString(roundPrices(recGames[5]));
            price2.setCharacterSize(50);
            price2.setFillColor(sf::Color::White);
            price2.setPosition(1200, 550);
            window.draw(price2);
            sf::Text price3;
            price3.setFont(font);
            price3.setString(roundPrices(recGames[8]));
            price3.setCharacterSize(50);
            price3.setFillColor(sf::Color::White);
            price3.setPosition(1200, 750);
            window.draw(price3);
            
            if (mapOrGraph == 1) {
                sf::Text time;
                time.setFont(font);
                time.setString("Run Time: " + times[0] + " microseconds");
                time.setCharacterSize(30);
                time.setFillColor(sf::Color::White);
                time.setPosition(1050, 950);
                window.draw(time);
            }
            else if (mapOrGraph == 2) {
                sf::Text time;
                time.setFont(font);
                time.setString("Run Time: " + times[1] + " microseconds");
                time.setCharacterSize(30);
                time.setFillColor(sf::Color::White);
                time.setPosition(1050, 950);
                window.draw(time);
            }
        }
        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
