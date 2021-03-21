// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

// TestApp.cpp
// Blackjack with OOP

#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <chrono>
#include <thread>
using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono;      // nanoseconds, system_clock, seconds


//create playing card class
class playing_card
{
    //card_name for displaying, card_number in deck, and card_value in game
    string card_name;
    int card_number, card_value;

public:
    void create_card(string card, int number, int value);
    int show_number();
    int retvalue();
};

void playing_card::create_card(string card, int number, int value)
{
    card_name = card;
    card_number = number;
    card_value = value;
}

int playing_card::show_number()
{
    return card_number;
}

int playing_card::retvalue()
{
    return card_value;
}


//creates deck with playing_card class, only for testing right now
void create_deck()
{
    int deck = 11;
    string card, suite;
    suite = "Hearts";
    int card_num = 1;

    playing_card _card[52];

    for (int i = 0; card_num < deck; i++)
    {
        card = to_string(card_num);
        _card[i].create_card(card + " of " + suite, card_num, card_num);
        
        cout << "\nvalue = " << _card[i].retvalue() << " num = " << _card[i].show_number();

        card_num++;
    }
}


//create player class
class player
{
    int hand_value = 0;
    int ace = 0;

public:
    int draw_card();
    void reduce_ace_val();
    void has_ace();
    int retvalue();
    int retace();
};

void player::reduce_ace_val()
{
    hand_value = hand_value - 10;
    ace--;
}

int player::retace()
{
    return ace;
}

void player::has_ace()
{
    ace++;
}

int player::draw_card()
{
    /*
    fix to draw a random card from deck and add to hand_value
    right now it just picks a random num from 1-11 to add to hand_value
    */
    srand(time(NULL));
    //srand initialized with runtime value of the function time, makes it random 
    //everytime I have input buffering between calls to the function 
    //will need a better random number generation in the future
    int max = 11;
    int min = 2;
    int range = max - min + 1;
    int card = rand() % range + min;
    hand_value = hand_value + card;
    return card;
}

int player::retvalue()
{
    return hand_value;
}


//declaration of functions
void start();
bool hit(int hand);
void show_winner(int dealer_hand, int player_hand);


//main function
int main()
{
    while (true)
    {
        start();
    }

    return 0;
}


//currently doesn't use player_card objects, just random nums to test
void start()
{
    player player1;
    player dealer;
    playing_card _card[52];

    int card1, card2, card, player_hand, dealer_hand;
    bool hit_me;
    bool stand = false;
    bool dealer_stand = false;
    //system("cls");

    int deck = 11;
    string card_name, hearts, diamonds, spades, clubs;
    hearts = "Hearts";
    int card_num = 1;

    //creates deck with playing_card objects
    for (int i = 0; card_num < deck; i++)
    {
        card_name = to_string(card_num);
        _card[i].create_card(card_name + " of " + hearts, card_num, card_num);

        cout << "\nvalue = " << _card[i].retvalue() << " num = " << _card[i].show_number();

        card_num++;
    }

    cout << "\n" << _card[3].retvalue() << endl;


    //player gets dealt card
    card1 = player1.draw_card();
    if (card1 == 11)
    {
        player1.has_ace();
    }
    cout << "First card: " << card1;
    sleep_for(1s);


    //player gets second card dealt
    card2 = player1.draw_card();
    if (card2 == 11)
    {
        player1.has_ace();
    }
    cout << "\nSecond card: " << card2;
    cout << "\nHand equals " << card1 << " + " << card2 << " = " << player1.retvalue();
    sleep_for(1s);


    //dealer shows first card
    card = dealer.draw_card();
    if (card == 11)
    {
        dealer.has_ace();
    }
    cout << "\n\nDealer drew a " << card;


    //handles if player wants to hit or stand
    while (player1.retvalue() <= 21 && stand == false)
    {
        hit_me = hit(player1.retvalue());

        if (hit_me)
        {
            card = player1.draw_card();
            if (card == 11)
            {
                player1.has_ace();
            }
            cout << "\nYou drew a " << card;
            //cout << "\n" << player1.retvalue();
        }
        else
        {
            stand = true;
        }
        if (player1.retvalue() > 21 && player1.retace() > 0)
        {
            player1.reduce_ace_val();
            cout << "\nyou had an ace so total has been reduced to " << player1.retvalue();
        }
    }
    if (player1.retvalue() > 21)
    {
        cout << "\n\nYour cards total "<< player1.retvalue() << " and went over 21, you lost.";
        cin.ignore();
        cin.get();
    }
    else 
    {
        while (!dealer_stand)
        {
            if (dealer.retvalue() < 17)
            {
                card = dealer.draw_card();
                if (card == 11)
                {
                    dealer.has_ace();
                }
                cout << "\n\nDealer drew a " << card;
                cout << "\nDealer Total = " << dealer.retvalue();
                cin.ignore();
                cin.get();
            }
            else
            {
                dealer_stand = true;
            }
            if (dealer.retvalue() > 21 && dealer.retace() > 0)
            {
                dealer.reduce_ace_val();
                cout << "\nDealer had an ace so total has been reduced to " << dealer.retvalue();
            }
        }

        //takes total values in hands and shows who won
        player_hand = player1.retvalue();
        dealer_hand = dealer.retvalue();
        show_winner(dealer_hand, player_hand);
    }
    
}

//asks player if they want to hit for another card or stand
bool hit(int hand)
{
    // need to implement error handling on bad inputs
    char option;
    cout << "\n\nYour hand is " << hand;
    cout << "\nWould you like to hit?(y/n): ";
    cin >> option;

    if (option == 'y')
    {
        return true;
    }
    else
    {
        return false;
    }
}


//handles logic for figuring out and displaying who won
void show_winner(int dealer_hand, int player_hand)
{
    cout << "\n\nPlayer Total = " << player_hand;
    cout << "\nDealer Total = " << dealer_hand;


    if (player_hand > 21 || (21 >= dealer_hand && dealer_hand > player_hand))
    {
        cout << "\n\ndealer had " << dealer_hand <<
            " and you had " << player_hand << ", you lose.";
    }
    else if (21 >= dealer_hand && dealer_hand == player_hand)
    {
        cout << "\n\nYou both had " << player_hand << " so it's a push";
    }
    else
    {
        cout << "\n\ndealer had " << dealer_hand <<
            " and you had " << player_hand << ", you win.";
    }

    cin.ignore();
    cin.get();
}