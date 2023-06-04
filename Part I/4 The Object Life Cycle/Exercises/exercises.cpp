#include <cstring>
#include <thread>

struct TimerClass {
    explicit TimerClass(const char* name) : timestamp {std::chrono::system_clock::now()} {
        strcpy(this->name, name);
    }

    TimerClass(const TimerClass& other) : timestamp {other.timestamp} {
        strcpy(name, other.name);
        strcat(name, " copied");
    }

    TimerClass(TimerClass&& other)  noexcept : timestamp {other.timestamp}{
        other.moved_from = true;
        strcpy(name, other.name);
        strcat(name, " moved");
    }

    TimerClass& operator=(const TimerClass& other) {
        if (this == &other) return *this;
        strcpy(name, other.name);
        strcat(name, " copy-assigned");
        timestamp = other.timestamp;
        return *this;
    }
    
    TimerClass& operator=(TimerClass&& other)  noexcept {
        if (this == &other) return *this;
        strcpy(name, other.name);
        strcat(name, " move-assigned");
        timestamp = other.timestamp;
        other.moved_from = true;
        return *this;
    }

    void setName(char* name) {
        strcpy(this->name, name);
    }

    void printTimeDiff() {
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end-timestamp;
        printf("Lifetime of timer %s %f\n",name,  diff.count());
    }

    ~TimerClass() {
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end-timestamp;
        if (!moved_from) {
            printf("Lifetime of timer %s %f\n",name,  diff.count());
        }

    }
private:
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    char name[100]{};
    bool moved_from = false;
};

void expensive_computation(TimerClass&& timer) {
    int n = 1;
    for (int i = 0; i < 10000000; i++) {
        n = (n * i * i) % 2021;
    }
}

int main() {
    using namespace std::chrono_literals;
    auto timer1 = TimerClass("Timer 1");
    auto timer3 = TimerClass("Timer 3");
    auto timer2 {timer1};
    auto timer4 = TimerClass("Timer 4");
    timer4 = timer3;
    auto timer5 = TimerClass("Timer 5");
    auto timer6 {std::move(timer3)};
    timer5 = std::move(timer1);
    timer2.printTimeDiff();
    expensive_computation(std::move(timer2));
}
