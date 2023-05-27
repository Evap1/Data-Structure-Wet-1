//
// Created by Eva Poluliakhov on 24/05/2023.
//

#ifndef WET1_USER_H
#define WET1_USER_H
#include "wet1util.h"
#include "AVL.h"

class User{
private:
    int userId;
    bool isVip;
    int viewsPerGenre[(int)Genre::NONE] = {0};
    int groupId;

public:
    explicit User(int userId,bool isVip);
    explicit User(int userId);
    void set_group_id(int id);
    int get_user_id() const;
    bool is_vip() const;
    int get_group_id() const;
    int get_views_per_genre(Genre type) const;
    void add_views_in_genre(Genre type, int nunOfViews);
    static const int NONE = -1;
};

// ___________________________________________Operator Overloading__________________________________________

bool operator>(const User &user1, const User &user2);
bool operator<(const User& user1, const User& user2);
bool operator==(const User& user1, const User& user2);


#endif //WET1_USER_H
