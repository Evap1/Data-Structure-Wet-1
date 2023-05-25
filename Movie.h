
#ifndef WET1_MOVIE_H
#define WET1_MOVIE_H

#include "wet1util.h"


class Movie{

    private:
    
    int movieId;
    Genre genre;
    bool vipOnly;
    int views;
    double rate;
    int ratersCont;

public:
    Movie(int id, Genre genreType, int viewsCount, bool isVip);
    ~Movie() = default;

    int getMovieId() const;
    Genre getGenre() const;
    bool isVipOnly() const;
    int getViews() const;
    double getRate() const;

    void add_views(int count);
    void add_rating(int rate);



};


#endif //WET1_MOVIE_H
