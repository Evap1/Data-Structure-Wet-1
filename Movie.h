
#ifndef WET1_MOVIE_H
#define WET1_MOVIE_H

#include "wet1util.h"
#include<iostream>


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
    Movie(const Movie& movie) = delete;
    ~Movie() =  default;

    int getMovieId() const;
    Genre getGenre() const;
    bool isVipOnly() const;
    int getViews() const;
    double getRate() const;

    void add_views(int count);
    void add_rating(int rate);

    void printMovie() const;


};


#endif //WET1_MOVIE_H
