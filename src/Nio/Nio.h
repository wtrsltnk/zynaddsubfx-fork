#ifndef NIO_H 
#define NIO_H
#include <string>
#include <set>

/**Interface to Nio Subsystem
 * 
 * Should be only externally included header */
class Nio
{
    public:
        static Nio &getInstance();
        ~Nio();

        bool start();
        void stop();

        int setDefaultSource(std::string name);
        int setDefaultSink(std::string name);

        bool setSource(std::string name);
        bool setSink(std::string name);

        void setPostfix(std::string post);
        std::string getPostfix(void) const;

        std::set<std::string> getSources() const;
        std::set<std::string> getSinks() const;

        std::string getSource() const;
        std::string getSink()   const;

        bool autoConnect;
    private:
        Nio();

        class InMgr     &in;
        class OutMgr    &out;
        class EngineMgr &eng;
        std::string postfix;
};

#endif
