#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>

class Hour
{
    int hours, minutes, seconds;
    static const int dan = 86400; // number of seconds
public:
    Hour();
    Hour(int hours, int minutes, int seconds);
    void SetNormalized(int hours, int minutes, int seconds);
    int GetHours() const;
    int GetMinutes() const;
    int GetSeconds() const;
    friend Hour &operator++(Hour &hour);
    friend Hour operator++(Hour &hour, int);
    friend Hour &operator--(Hour &hour);
    friend Hour operator--(Hour &hour, int);
    friend Hour operator+(Hour &hour, int sec);
    friend Hour operator-(Hour &hour, int sec);
    friend Hour operator+=(Hour &hour, int sec);
    friend Hour operator-=(Hour &hour, int sec);
    friend int operator-(const Hour &hour1, const Hour &hour2);
    friend std::ostream &operator<<(std::ostream &tok, const Hour&hour);
};

void Hour::SetNormalized(int hours, int minutes, int seconds)
{
    int temp = hours * 3600 + minutes * 60 + seconds;
    if (temp >= dan)
        temp = temp % dan; // if we 'overshot' the day
    if (temp < 0)
        temp = dan + temp % dan; // in case we have a negative shift (time)
    this->hours = temp / 3600;
    this->minutes = (temp % 3600) / 60;
    this->seconds = temp % 60;
}
Hour::Hour() : hours(0), minutes(0), seconds(0) {}
Hour::Hour(int hours, int minutes, int seconds)
    : hours(hours), minutes(minutes), seconds(seconds)
{
    SetNormalized(hours, minutes, seconds);
}

int Hour::GetHours() const
{
    return hours;
}
int Hour::GetMinutes() const
{
    return minutes;
}
int Hour::GetSeconds() const
{
    return seconds;
}


int operator-(const Hour &hour1, const Hour &hour2)
{
    int time1 = hour1.GetHours() * 3600 + hour1.GetMinutes() * 60 + hour1.GetSeconds();
    int time2 = hour2.GetHours() * 3600 + hour2.GetMinutes() * 60 + hour2.GetSeconds();
    return time1 - time2;
}

Hour operator +(Hour &hour1, int sec)
{
    Hour Hour2(hour1.hours, hour1.minutes, hour1.seconds + sec);
    return Hour2;
}
Hour operator -(Hour &hour, int sec)
{
    Hour hour2(hour.hours, hour.minutes, hour.seconds - sec);
    return hour2;
}
Hour operator+=(Hour &hour, int sec)
{
    hour.seconds += sec;
    hour.SetNormalized(hour.hours, hour.minutes, hour.seconds);
    return hour;
}
Hour operator-=(Hour &hour, int sec)
{
    hour.seconds -= sec;
    hour.SetNormalized(hour.hours, hour.minutes, hour.seconds);
    return hour;
}
std::ostream &operator<<(std::ostream &tok, const Hour&hour)
{
    tok << std::setw(2) << std::setfill('0') << hour.hours << ":";
    tok << std::setw(2) << std::setfill('0') << hour.minutes ;
    return tok;
}


int getTime(std::string mode){
auto timeGet = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(timeGet);
    std::string s=std::ctime(&time);
    if(mode=="hours")
    return std::stoi(s.substr(11,2));
    return std::stoi(s.substr(14,2));
}

int main()
{
    // Get today's time
    auto timeGet = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(timeGet);
    std::string s=std::ctime(&time);
    std::string day=s.substr(0,3);
    std::string monthDay=s.substr(4,6);
    std::string year=s.substr(20,18);
    std::string fullDate=monthDay+" "+year;

    std::cout<<"WORK TIME"<<std::endl;
    std::cout<<"-----------"<<std::endl;
    std::cout<<fullDate<<std::endl;

    int hours, minutes;
    // load previous time from file
    std::ifstream inFile("time.dat", std::ios::binary);
    if (!inFile){
    std::cout<<"Error: File time.dat was not found!"<<std::endl;
    return 0;
    }

    if(day=="Mon"){
    inFile.seekg(0 * sizeof(int));
    inFile.read(reinterpret_cast<char*>(&hours), sizeof hours);
    inFile.seekg(1 * sizeof(int));
    inFile.read(reinterpret_cast<char*>(&minutes), sizeof minutes);
    }
    if(day=="Tue"){
    inFile.seekg(2 * sizeof(int));
    inFile.read(reinterpret_cast<char*>(&hours), sizeof hours);
    inFile.seekg(3 * sizeof(int));
    inFile.read(reinterpret_cast<char*>(&minutes), sizeof minutes);
    }
    if(day=="Wed"){
    inFile.seekg(4 * sizeof(int));
    inFile.read(reinterpret_cast<char*>(&hours), sizeof hours);
    inFile.seekg(5 * sizeof(int));
    inFile.read(reinterpret_cast<char*>(&minutes), sizeof minutes);
    }
    if(day=="Thu"){
    inFile.seekg(6 * sizeof(int));
    inFile.read(reinterpret_cast<char*>(&hours), sizeof hours);
    inFile.seekg(7 * sizeof(int));
    inFile.read(reinterpret_cast<char*>(&minutes), sizeof minutes);
    }
    if(day=="Fri"){
    inFile.seekg(8 * sizeof(int));
    inFile.read(reinterpret_cast<char*>(&hours), sizeof hours);
    inFile.seekg(9 * sizeof(int));
    inFile.read(reinterpret_cast<char*>(&minutes), sizeof minutes);
    }
    if(day=="Sat"){
    inFile.seekg(10 * sizeof(int));
    inFile.read(reinterpret_cast<char*>(&hours), sizeof hours);
    inFile.seekg(11 * sizeof(int));
    inFile.read(reinterpret_cast<char*>(&minutes), sizeof minutes);
    }
    if(day=="Sun"){
    inFile.seekg(12 * sizeof(int));
    inFile.read(reinterpret_cast<char*>(&hours), sizeof hours);
    inFile.seekg(13 * sizeof(int));
    inFile.read(reinterpret_cast<char*>(&minutes), sizeof minutes);
    }

    inFile.close();

    // set main time class
    Hour h(hours,minutes,0);

    for(;;){
    std::string mode;
    std::cin>>mode;
    int start_hours, start_minutes, end_hours,end_minutes;

    // start of work time
    if(mode=="s"){
    start_hours=getTime("hours");
    start_minutes=getTime("minutes");
    }
    Hour h1(start_hours,start_minutes,0);

    // end of work time
    if(mode=="e"){
    end_hours=getTime("hours");
    end_minutes=getTime("minutes");
    Hour h2(end_hours,end_minutes,0);

    // total work time
    h+=(h2-h1);
    std::cout<<h<<std::endl;

    // save copy of previous times to array
    std::ifstream inFile("time.dat", std::ios::binary);
    if (!inFile){
    std::cout<<"Error: File time.dat was not found!"<<std::endl;
    return 0;
    }
    int arr[14];
    for (int i = 0; i<14; i++) {
    int number;
    inFile.seekg(i * sizeof(int));
    inFile.read(reinterpret_cast<char*>(&number), sizeof number);
    arr[i]=number;
    }
    inFile.close();

    // add new time to current day in array
    if(day=="Mon")
        arr[0]=h.GetHours(), arr[1]=h.GetMinutes();
    if(day=="Tue")
        arr[2]=h.GetHours(), arr[3]=h.GetMinutes();
    if(day=="Wed")
        arr[4]=h.GetHours(), arr[5]=h.GetMinutes();
    if(day=="Thu")
        arr[6]=h.GetHours(), arr[7]=h.GetMinutes();
    if(day=="Fri")
        arr[8]=h.GetHours(), arr[9]=h.GetMinutes();
    if(day=="Sat")
        arr[10]=h.GetHours(), arr[11]=h.GetMinutes();
    if(day=="Sun")
        arr[12]=h.GetHours(), arr[13]=h.GetMinutes();

    // save time to file
    std::ofstream outFile("time.dat", std::ios::binary);
    if (!outFile){
    std::cout<<"Error: File time.dat was not found!"<<std::endl;
    return 0;
    }
    outFile.write(reinterpret_cast<char*>(arr), sizeof arr);
    outFile.close();

    }

    // show full time in hours
    if(mode=="t"){
        std::cout<<"\nToday: "<<std::setprecision(4)<<h.GetHours()+h.GetMinutes()/60.<<"h\n"<<std::endl;
    }

    // show weekly time
    if(mode=="w"){

    std::ifstream inFile("time.dat", std::ios::binary);
    if (!inFile){
    std::cout<<"Error: File time.dat was not found!"<<std::endl;
    return 0;
    }

    int hours,minutes;
    double sumHours=0, sumMinutes=0;

   std::cout<<"\nMon: ";
   inFile.seekg(0 * sizeof(int));
   inFile.read(reinterpret_cast<char*>(&hours), sizeof hours);
   sumHours+=hours;
   inFile.seekg(1 * sizeof(int));
   inFile.read(reinterpret_cast<char*>(&minutes), sizeof minutes);
   sumMinutes+=minutes;
   std::cout<<std::setprecision(4)<<hours+minutes/60.<<std::endl;

   std::cout<<"Tue: ";
   inFile.seekg(2 * sizeof(int));
   inFile.read(reinterpret_cast<char*>(&hours), sizeof hours);
   sumHours+=hours;
   inFile.seekg(3 * sizeof(int));
   inFile.read(reinterpret_cast<char*>(&minutes), sizeof minutes);
   sumMinutes+=minutes;
   std::cout<<hours+minutes/60.<<std::endl;

   std::cout<<"Wed: ";
   inFile.seekg(4 * sizeof(int));
   inFile.read(reinterpret_cast<char*>(&hours), sizeof hours);
   sumHours+=hours;
   inFile.seekg(5 * sizeof(int));
   inFile.read(reinterpret_cast<char*>(&minutes), sizeof minutes);
   sumMinutes+=minutes;
   std::cout<<hours+minutes/60.<<std::endl;

   std::cout<<"Thu: ";
   inFile.seekg(6 * sizeof(int));
   inFile.read(reinterpret_cast<char*>(&hours), sizeof hours);
   sumHours+=hours;
   inFile.seekg(7 * sizeof(int));
   inFile.read(reinterpret_cast<char*>(&minutes), sizeof minutes);
   sumMinutes+=minutes;
   std::cout<<hours+minutes/60.<<std::endl;

   std::cout<<"Fri: ";
   inFile.seekg(8 * sizeof(int));
   inFile.read(reinterpret_cast<char*>(&hours), sizeof hours);
   sumHours+=hours;
   inFile.seekg(9 * sizeof(int));
   inFile.read(reinterpret_cast<char*>(&minutes), sizeof minutes);
   sumMinutes+=minutes;
   std::cout<<hours+minutes/60.<<std::endl;

   std::cout<<"Sat: ";
   inFile.seekg(10 * sizeof(int));
   inFile.read(reinterpret_cast<char*>(&hours), sizeof hours);
   sumHours+=hours;
   inFile.seekg(11 * sizeof(int));
   inFile.read(reinterpret_cast<char*>(&minutes), sizeof minutes);
   sumMinutes+=minutes;
   std::cout<<hours+minutes/60.<<std::endl;

   std::cout<<"Sun: ";
   inFile.seekg(12 * sizeof(int));
   inFile.read(reinterpret_cast<char*>(&hours), sizeof hours);
   sumHours+=hours;
   inFile.seekg(13 * sizeof(int));
   inFile.read(reinterpret_cast<char*>(&minutes), sizeof minutes);
   sumMinutes+=minutes;
   std::cout<<hours+minutes/60.<<std::endl;

   std::cout<<"Week: "<<sumHours+sumMinutes/60<<"h\n"<<std::endl;

  inFile.close();

    }

    // show all commands
    if(mode=="help"){
        std::cout<<"\nAll commands: "<<std::endl;
        std::cout<<"s - start work"<<std::endl;
        std::cout<<"e - end work"<<std::endl;
        std::cout<<"t - today time"<<std::endl;
        std::cout<<"w - week time"<<std::endl;
        std::cout<<"0 - reset week time"<<std::endl;
        std::cout<<"help - show all commands\n"<<std::endl;
    }

     // Reset time, new work week
     if(mode=="0"){
     std::ofstream outFile("time.dat", std::ios::binary);
     if (!outFile){
    std::cout<<"Error: File time.dat was not found!"<<std::endl;
    return 0;
    }
     int arr[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
     outFile.write(reinterpret_cast<char*>(arr), sizeof arr);
     outFile.close();
        break;
     }
    }


    std::string c;
    for(;;){
        std::cout<<std::endl<<"Finish y/n: ";
    std::cin>>c;
           if(c=="y"||c=="Y"||c=="n"||c=="N")
            break;
    }


    return 0;
}


