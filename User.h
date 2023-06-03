//
// Created by Eva Poluliakhov on 24/05/2023..
//

#ifndef WET1_USER_H
#define WET1_USER_H
#include "wet1util.h"
#include "AVL.h"

class Group;

class User {
private:
    int userId;
    bool isVip;
    int viewsPerGenre [(int)Genre::NONE + 1] = {0};
    int groupWatchWhenJoin [(int)Genre::NONE + 1] = {0};
    int groupId;
    Group* group;

public:
    static const int NONE = -1;

    explicit User(int userId,bool isVip);
    explicit User(int userId);
    ~User() = default;

    void set_group_id(int id);
    void set_group_ptr(Group* other);
    void set_group_ptr_null(std::nullptr_t); //in case of leaving group
    void set_views_as_group_join(Genre type, int amount);

    int get_user_id() const;
    int get_group_id() const;
    Group* get_group_ptr() const;
    int get_views_per_genre(Genre type) const;
    int get_views_when_leave(Genre type) const;

    void leave_group();
    bool is_vip() const;
    void add_views_in_genre(Genre type);

    void leave_group(int groupWatchComedy, int groupWatchDrama,int groupWatchAction,
                           int groupWatchFantasy, int groupWatchNone);

};

// ___________________________________________Operator Overloading__________________________________________

bool operator>(const User &user1, const User &user2);
bool operator<(const User& user1, const User& user2);
bool operator==(const User& user1, const User& user2);



class UserPtrCompare{
    public:
    UserPtrCompare() = default;
    bool operator()(const User* userPtr1, const User* userPtr2, Equality kind) const
    {
        if(kind == Equality::LESS)
            return userPtr1->get_user_id() < userPtr2->get_user_id();   

        if(kind == Equality::EQUAL)
            return userPtr1->get_user_id() == userPtr2->get_user_id();

        return userPtr1->get_user_id() > userPtr2->get_user_id();
    }
};

#endif //WET1_USER_H
