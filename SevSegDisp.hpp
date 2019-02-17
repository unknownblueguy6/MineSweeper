#pragma once

#include <vector>
#include <cmath>
#include "setup.hpp"
#include "buffer.hpp" 
#include "colour.hpp"

const std::string numbers[10][5] = {
    {
        "╻══╻",
        "║  ║",
        "║  ║",
        "║  ║",
        "╹══╹"
    },

    {
        "   ╻",
        "   ║",
        "   ║",
        "   ║",
        "   ╹"
    },

    {
        " ══╻",
        "   ║",
        "╻══╹",
        "║   ",
        "╹══ "
    },

    {
        " ══╻",
        "   ║",
        " ══║",
        "   ║",
        " ══╹"
    }, 

    {
        "╻  ╻",     
        "║  ║",     
        "╹══║",     
        "   ║",     
        "   ╹"
    },

    {
        "╻══ ",
        "║   ",
        "╹══╻",
        "   ║",
        " ══╹"
    },

    {
        "╻══ ",
        "║   ",
        "║══╻",
        "║  ║",
        "╹══╹"
    },

    {
        " ══╻",
        "   ║",
        "   ║",
        "   ║",
        "   ╹"
    },

    {
        "╻══╻",
        "║  ║",
        "║══║",
        "║  ║",
        "╹══╹"
    },
    
    {
        "╻══╻",
        "║  ║",
        "╹══║",
        "   ║",
        " ══╹"
    }

};

class SevSegDisp{
    public:
        SevSegDisp(int,int);
        
        void operator ++(){
            if(number > int(std::pow(10, noOfDigits)) - 1) return;
            else ++number;
        }

        void operator --(){
            if(number) --number;
        }
        
        void set(int);
        void update();

    
    private:
    int noOfDigits;
    int number;
} flagDisp(5, 0), timeDisp(5, 0);

SevSegDisp::SevSegDisp(int dig, int n){
    noOfDigits = dig;
    number = n;
}

void SevSegDisp::set(int n){
    number = n;
}

void SevSegDisp::update(){
    int homeLine = writeBuf.getCurrLine();
    int nos[noOfDigits];
    int num = number;
    int dig = noOfDigits; 
    while(num || dig){
        nos[dig-1] = num % 10;
        num /= 10;
        --dig;
    }
    for(auto no : nos){
        for(auto str : numbers[no]) writeBuf << red_fg << str << " " << endl << reset;
        writeBuf.goToLine(homeLine); 
    }
}