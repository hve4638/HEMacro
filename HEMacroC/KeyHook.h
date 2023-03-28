#pragma once
#include <Windows.h>
#include <iostream>
#define MAGICFN_RANGE 256
#define VKCODE DWORD

namespace KeyHook {
    typedef LRESULT(*KeyEvent)(PKBDLLHOOKSTRUCT);
    #define KeyEventLambda(x) [](PKBDLLHOOKSTRUCT x) -> LRESULT

    namespace InputEditer {
        void Down(INPUT& input, WORD vk);
        void Up(INPUT& input, WORD vk);
    }

    class MagicFnEvents {
    private:
    public:
        KeyEvent pressEvents[MAGICFN_RANGE] = { 0, };
        KeyEvent releaseEvents[MAGICFN_RANGE] = { 0, };
        WORD hotkey[MAGICFN_RANGE] = { 0, };

        void setPressAndReleaseEvent(WORD, KeyEvent, KeyEvent);
    };

    class IKeyListener {
    public:
        virtual LRESULT pressing(PKBDLLHOOKSTRUCT) = 0;
        virtual LRESULT releasing(PKBDLLHOOKSTRUCT) = 0;
    };

    class KeyListener : public IKeyListener {
        MagicFnEvents* m_magicFnEvents;
    public:
        void setMagicFnEvents(MagicFnEvents*);

        LRESULT pressing(PKBDLLHOOKSTRUCT) override;
        LRESULT releasing(PKBDLLHOOKSTRUCT) override;
    };

    class KeyHooker {
    private:
        bool m_magicFnEnabled = false;
        IKeyListener* m_keyListener = NULL;

    public:
        void setKeyListener(IKeyListener*);
        LRESULT onHook(int, WPARAM, LPARAM);
        void run();
    };

    enum InputQueueState {
        WAITING,
        READY,
        FINISHED,
    };
    enum InputNodeFlags {
        DOWN,
        UP,
    };

    typedef struct Node {
        DWORD vkCode;
        InputNodeFlags flag;
        Node* next;
    } InputQueueNode;

    class InputQueue {
    private:
        InputQueueNode* m_firstNode = NULL;
        InputQueueNode* m_lastNode = NULL;
        InputQueueState m_state = FINISHED;
        int m_count = 0;

        void pushNode(InputQueueNode*);
        void sendFirstNode();
    public:
        static InputQueue Instance;

        int count();
        InputQueueState state();
        bool isEmpty();
        void pushInput(size_t, INPUT[]);
        void push(VKCODE, InputNodeFlags);
        void pop();
        bool match(VKCODE, InputNodeFlags);

        InputQueueNode* rawNodeBegin();
    };


    void RunKeyHook(KeyHooker*);
}