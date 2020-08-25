
typedef struct Message{

    char body[140];
    char src[50];
    char dst[50];
    char upperTopic[50];
    int next;
    
}Message;

typedef struct Topic {
    
    char name[50];
    char upperThread[50];
    char owner[50];
    int next;
    
}Topic;

typedef struct Thread {

    char name[50];
    char owner[20];
    int next;
    
}Thread;
