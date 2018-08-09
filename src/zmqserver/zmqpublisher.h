// Copyright (c) 2018 Tadhg Riordan, Zcoin Developer
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_ZMQ_ZMQPUBLISHER_H
#define BITCOIN_ZMQ_ZMQPUBLISHER_H

#include "zmqabstract.h"
#include "univalue.h"

class CBlockIndex;

class CZMQAbstractPublisher : public CZMQAbstract
{
public:
    bool Initialize();
    void Shutdown();

    bool Execute();
    bool Publish();

    virtual void SetMethod() = 0;
    virtual void SetTopic() = 0;

protected:
    std::string method;
    UniValue request;
    UniValue publish;

};

/* Event classes. Each one is a specific notifier in ValidationInterface */
class CZMQBlockEvent : virtual public CZMQAbstractPublisher
{
    /* Data related to a new block (updatedblocktip)
    */
public:
    bool NotifyBlock(const CBlockIndex *pindex);
};


class CZMQTransactionEvent : virtual public CZMQAbstractPublisher
{
    /* Data related to a new transaction
    */
public:
    bool NotifyTransaction(const CTransaction &transaction);
};

class CZMQConnectionsEvent : virtual public CZMQAbstractPublisher
{
    /* Updated connection count
    */
public:
    bool NotifyConnections();
};

class CZMQStatusEvent : virtual public CZMQAbstractPublisher
{
    /* Updated blockchain sync status
    */
public:
    bool NotifyStatus();
};

/* Topics. inheriting from an event class implies publishing on that event. 
   'method' string is the API method called in client-api/ 
*/
class CZMQBlockDataTopic : public CZMQBlockEvent
{
public:
    void SetTopic(){ topic = "address";};
    void SetMethod(){ method= "block";};    
};

class CZMQAvgBlockTimeTopic : public CZMQBlockEvent
{
public:
    void SetTopic(){ topic = "avgblocktime";};
    void SetMethod(){ method= "avgblocktime";};   
};

class CZMQBlockInfoTopic : public CZMQBlockEvent,
                           public CZMQConnectionsEvent,
                           public CZMQStatusEvent
{
public:
    void SetTopic(){ topic = "block";};
    void SetMethod(){ method= "blockchain";};   
};

class CZMQBalanceTopic : public CZMQBlockEvent, 
                         public CZMQTransactionEvent
                         
{
public:
    void SetTopic(){ topic = "balance";};
    void SetMethod(){ method= "balance";};
};

class CZMQTransactionTopic : public CZMQTransactionEvent
{
public:
    void SetTopic(){ topic = "transaction";};
    void SetMethod(){ method= "transaction";};
};

class CZMQSettingsUpdatePublisher : public CZMQAbstractPublisher
{
public:
    bool NotifySettingsUpdate();
    void SetMethod(){ method= "settings";};
    void SetTopic(){ topic = "settings";};
};

#endif // BITCOIN_ZMQ_ZMQPUBLISHER_H
