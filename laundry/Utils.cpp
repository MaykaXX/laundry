#include "Utils.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "Laundry.h"


void slowPrint(const std::string& text, int delay) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << std::endl;
}

void showMaykaRetroBanner() {
    std::string banner = R"(
                                                                  .---.        ____      ___    _ ,---.   .--. ______     .-------. 
                      | ,_|      .'  __ `. .'   |  | ||    \  |  ||    _ `''. |  _ _   \      \   \   /  / 
                    ,-./  )     /   '  \  \|   .'  | ||  ,  \ |  || _ | ) _  \| ( ' )  |       \  _. /  '  
                    \  '_ '`)   |___|  /  |.'  '_  | ||  |\_ \|  ||( ''_'  ) ||(_ o _) /        _( )_ .'   
                     > (_)  )      _.-`   |'   ( \.-.||  _( )_\  || . (_) `. || (_,_).' __  ___(_ o _)'    
                    (  .  .-'   .'   _    |' (`. _` /|| (_ o _)  ||(_    ._) '|  |\ \  |  ||   |(_,_)'     
                     `-'`-'|___ |  _( )_  || (_ (_) _)|  (_,_)\  ||  (_.\.' / |  | \ `'   /|   `-'  /      
                      |        \\ (_ o _) / \ /  . \ /|  |    |  ||       .'  |  |  \    /  \      /       
                      `--------` '.(_,_).'   ``-'`-'' '--'    '--''-----'`    ''-'   `'-'    `-..-'        
                                                                                       
        )";
    std::cout << "\033[1;36m"; // Голубой стиль
    std::cout << banner;
    std::cout << "\033[0m\n";
}

void print_name(const std::string& name) {
    std::cout << name << ": ";
}

void print_user_name(const std::string& name) {
    std::string n = "👤" + name + ": ";
    std::cout << n;
}