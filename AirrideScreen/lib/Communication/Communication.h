#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <functional>
#include <WString.h>
#include <vector>

#include "../ISerial/ISerial.h"

using Callback = std::function<void(String message)>;

class Communication {
    public:
    Communication(ISerial& serial): nextId(1), serial(serial) {};
    ~Communication();
    unsigned int Subscribe(Callback callback);
    void Unsubscribe(int id);
    void Notify(String message);
    void CheckForMessage();
    private:
    struct Subscription {
        unsigned int id;
        Callback callback;
    };
    std::vector<Subscription> subscribers;
    unsigned int nextId;
    ISerial& serial;

};


#endif //COMMUNICATIONDISTRIBUTOR_H