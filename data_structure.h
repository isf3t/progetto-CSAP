
typedef struct Message{

    char* Body[140];
    char* src[50];
    char* dst[50];
    struct Message* next;
    
}Message;

typedef struct Thread {

    char* name[50];
    char* upperTopic[50];
    struct Thread* next;
    Message* messages;
    
}Thread;

typedef struct Topic {
    
    char* name[50];
    char* owner[20];
    struct Topic* next;
    Thread* threads;
    
}Topic;
