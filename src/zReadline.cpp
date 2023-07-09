#include <cstring>
#include<iostream>
#include <termios.h>
#include "zPreDefinition.h"
#include "zReadline.h"
#include <filesystem>


using namespace std;

void zReadline::read()
{
    int inputLength=0;
    int cursorLength=0;

    struct termios new_setting,init_setting;
    tcgetattr(0,&init_setting);
    new_setting=init_setting;
    //get termios setting and save it
    new_setting.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0,TCSANOW,&new_setting);
    // 屏蔽输入提示,关闭缓冲

    string inputOrder;
    cout<<title_format<<remindWords<<"\033[0m";
    while(1){
        //cout<<title_format<<remindWords;
        char tempOrder;
        tempOrder = getchar();
        if(tempOrder == '\n')
        {
            cout<<endl<<title_format<<remindWords<<"\033[0m"<<normal_format<<"Your input:"<<inputOrder<<"\033[0m"<<endl;
            if(inputOrder == "exit"){
                cout<<exit_format<<exitWords<<"\033[0m";
                break;
            }
            cout<<title_format<<remindWords<<"\033[0m";
            inputOrder.erase();
            inputLength=0;
            cursorLength=0;
        }
        else if(tempOrder == '\b' || tempOrder == 0x7f)
        {
            if(!inputOrder.empty() && cursorLength>0){
                inputOrder.erase(cursorLength-1,1);
                cursorLength-=1;
                inputLength-=1;
                std::cout<<"\b \b";
            }
        }//退格符
        else if(tempOrder == 0x1B)
        {
            getchar();
            char direction = getchar();
            if(direction == 0x44 && cursorLength>0){
                cout<<"\033[1D";
                cursorLength-=1;

            }//左键
            else if(direction == 0x43 && cursorLength<inputLength){
                cout<<"\033[1C";
                cursorLength+=1;
            }
        }
        else
        {
            cout<<tempOrder;
            inputOrder.insert(cursorLength,1,tempOrder);
            cursorLength+=1;
            inputLength+=1;

        }
    }
    
    tcsetattr(0,TCSANOW,&init_setting);
    return;
}
