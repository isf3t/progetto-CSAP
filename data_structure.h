
typedef struct Message{

    char body[140];
    char src[50];
    char dst[50];
    char upperTopic[50];
    struct Message* next;
    
}Message;

typedef struct Topic {
    
    char name[50];
    char upperThread[50];
    struct Topic* next;
    
}Topic;

typedef struct Thread {

    char name[50];
    char owner[20];
    struct Thread* next;
    
}Thread;
