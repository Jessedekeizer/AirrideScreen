#ifndef COMMUNICATIONDISTRIBUTOR_H
#define COMMUNICATIONDISTRIBUTOR_H
#include <functional>
#include <arduino.h>
#include <vector>

using Callback = std::function<void(String message)>;

class CommunicationDistributor {
    public:
    CommunicationDistributor(): nextId(1){};
    ~CommunicationDistributor();
    unsigned int Subscribe(Callback callback);
    void Unsubscribe(int id);
    void Notify(String message);
    private:
    struct Subscription {
        unsigned int id;
        Callback callback;
    };
    std::vector<Subscription> subscribers;
    unsigned int nextId;
};


#endif //COMMUNICATIONDISTRIBUTOR_H