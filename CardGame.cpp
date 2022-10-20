

#include "FinalProject.h"
#include <nana/push_ignore_diagnostic>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/timer.hpp>
#include <nana/audio/player.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/threads/pool.hpp>
#include <nana/gui/place.hpp>
#include <map>
#include <list>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class CardGame{

    public:
        /*define vector to and images to store*/
        vector<nana::paint::image> v = {};
        nana::paint::image back, card1, card2, card3, card4, card5, card6, card7, card8, card9, card10;
        nana::paint::image iPointer, iPointer1, iPointer2, iPointer3, iPointer4, iPointer5, iPointer6, iPointer7, iPointer8, iPointer9, iPointer10;
        vector <nana::paint::image> vecCard = { card1, card2, card3, card4, card5, card6, card7, card8, card9, card10, 
        card1, card2, card3, card4, card5, card6, card7, card8, card9, card10};
    
        CardGame() {
            back = nana::paint::image("backcard-bmp.bmp"); //add background
            //add images
            card1 = nana::paint::image("2C-bmp.bmp"); 
            card2 = nana::paint::image("4D-bmp.bmp");
            card3 = nana::paint::image("6S-bmp.bmp");
            card4 = nana::paint::image("7H-bmp.bmp");
            card5 = nana::paint::image("8H-bmp.bmp");
            card6 = nana::paint::image("10H-bmp.bmp");
            card7 = nana::paint::image("AS-bmp.bmp");
            card8 = nana::paint::image("JD-bmp.bmp");
            card9 = nana::paint::image("KC-bmp.bmp");
            card10 = nana::paint::image("QS-bmp.bmp");
          
            
            //pointer to cards
            iPointer1 = card1;
            iPointer2 = card2;
            iPointer3 = card3;
            iPointer4 = card4;
            iPointer5 = card5;
            iPointer6 = card6;
            iPointer7 = card7;
            iPointer8 = card8;
            iPointer9 = card9;
            iPointer10 = card10;


            //add them to vector
            v.push_back(iPointer1);
            v.push_back(iPointer2);
            v.push_back(iPointer3);
            v.push_back(iPointer4);
            v.push_back(iPointer5);
            v.push_back(iPointer6);
            v.push_back(iPointer7);
            v.push_back(iPointer8);
            v.push_back(iPointer9);
            v.push_back(iPointer10);
        }
        //getter
        vector <nana::paint::image> const& getV() {
       
            return v;
        }
        //getter
        vector <nana::paint::image> const& getImage() {
            vecCard = {  card1, card2, card3, card4, card5, card6, card7, card8, card9, card10,
            card1, card2, card3, card4, card5, card6, card7, card8, card9, card10};
            return vecCard;
        }

};

//function definition
int click(int a);
//definition of global variables
multimap <int, nana::paint::image> cardsM;
multimap <int, nana::paint::image> iter;
vector<nana::paint::image> iterCard;

int cl;
int score = 0;
int finishGame = 0;
int numClicks = 0;
//start of the program
int main()
{
    using namespace nana;
    //define function variables
    vector <nana::paint::image> vClicked;
    CardGame card;

    vector<nana::paint::image> vc = card.getImage();
    vector<nana::paint::image> v = card.getV();
    random_shuffle(vc.begin(), vc.end());

    nana::paint::image back;

    cardsM.insert(pair <int, nana::paint::image>(1, card.card1));
    cardsM.insert(pair <int, nana::paint::image>(2, card.card2));
    cardsM.insert(pair <int, nana::paint::image>(3, card.card3));
    cardsM.insert(pair <int, nana::paint::image>(4, card.card4));
    cardsM.insert(pair <int, nana::paint::image>(5, card.card5));
    cardsM.insert(pair <int, nana::paint::image>(6, card.card6));
    cardsM.insert(pair <int, nana::paint::image>(7, card.card7));
    cardsM.insert(pair <int, nana::paint::image>(8, card.card8));
    cardsM.insert(pair <int, nana::paint::image>(9, card.card9));
    cardsM.insert(pair <int, nana::paint::image>(10, card.card10));


    //create audio files
    nana::audio::player playerCs("click-sound.wav");
    nana::audio::player matchSound("applause.wav");
    nana::audio::player endGame("gameover.wav");

    nana::threads::pool pool(1);

    form fm(API::make_center(1000, 1000));

    fm.div("<Mycard grid=[4,5] margin=[5,110,5,110] gap=20>");

    fm.caption(("Hamza C++ 2021 - FINAL P"));

    drawing dw(fm);


    static std::map <char, button*> cards;
    std::list<button> buttons;

  
    back = nana::paint::image("backcard-bmp.bmp"); 


   //variables used inside loop
    int position[20] = { 0, 1, 2, 3 ,4 ,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 };
    int j = 0;
    int remove = 0;
    int lClick = 0;
    int choice = 0;
  
    string name;
   
        for (auto i : position) {
            buttons.emplace_front((fm).handle());

            auto& card_btn = buttons.front();
            cards[i] = &card_btn;


            card_btn.icon(back);
            fm["Mycard"] << card_btn;

            vector <nana::paint::image> flippedCards;


            card_btn.events().click(pool_push(pool, [&, i]() {

                card_btn.icon(vc.at(i));
                iterCard.push_back(vc.at(i));

                click(i);
                playerCs.play();

                if (lClick != 0) {
                    //compare user's card click with previous card click
                    if (vc.at(i) == iterCard.at(lClick)) {
                        cout << "Matched " << endl;
                        card_btn.icon(vc.at(i)).close();    //remove card after matching
                        card_btn.icon(vc.at(lClick)).close();
                        //matchSound.play();  //matching sound
                        score += 2;     //user score
                        finishGame++;   //increments after every card is removed
                    }
                    else {
                        card_btn.icon(back);
                    }

                }
                //when it is equal to 20 all cardsd have been removed
                if (finishGame == 20) {
                    endGame.play();     //end of game sound
                    /*Display user score and number of clikcs to the console*/
                    cout << "It took " << numClicks << "  Clicks to finish the game" << endl;
                    cout << "Your score is " << score << endl;
                    cout << "Enter your name to save your score" << endl;
                    cin >> name;    //saves user name
                    cout << "\n " << endl;
                    cout << " \n" << endl;
                    ofstream outfile("UserScore.txt");  //saves the user name, score and number of clicks in userScore file
                    outfile << "Name " + name << endl;
                    outfile <<  numClicks << endl;
                    outfile <<  score << endl;
                    outfile.close();
                    

                }
             
                lClick = i;
               
                }));



        }

        fm.collocate();

        fm.show();

        exec();
        fm.close();
       
    }



int click(int a) {
    
    std::cout << "I Flipped Card # : " << a + 1 << std::endl;
    numClicks++;
    return a;
}
