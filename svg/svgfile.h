﻿#ifndef SVGFILE_H_INCLUDED
#define SVGFILE_H_INCLUDED
///Source TP
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <sstream>
#include <cstdio>
#include <map>
#include <cmath>
#include <set>
#include "../physique/coords.h"

constexpr char defcol[] = "black";

class Svgfile
{
public:
    Svgfile(std::string _filename = "output.svg", int _width=1000, int _height=800);
    ~Svgfile();

    void addDisk(double x, double y, double r, std::string color=defcol);
    void addCircle(double x, double y, double r, double ep, std::string color=defcol);

    void addTriangle(double x1, double y1, double x2, double y2, double x3, double y3, std::string colorFill, double thickness, std::string colorStroke);
    void addTriangle(double x1, double y1, double x2, double y2, double x3, double y3, std::string colorFill=defcol);

    void addRectangle(double x, double y, double L, double l, std::string colorFill);

    void addPallelepipede(const Coords c1, const Coords c2, const Coords c3, const Coords c4, std::string colorFill);

    void addLine(Coords c1,Coords c2,std::string color=defcol);
    void addLine(double x1, double y1, double x2, double y2, std::string color=defcol);
    void addCross(double x, double y, double span, std::string color=defcol);

    void addText(double x, double y, std::string text, std::string color=defcol);
    void addText(double x, double y, double val, std::string color=defcol);

    void addGrid(double span=100.0, bool numbering=true, std::string color="lightgrey");

    static std::string makeRGB(int r, int g, int b);

    /// Type non copiable
    Svgfile(const Svgfile&) = delete;
    Svgfile& operator=(const Svgfile&) = delete;

    static bool s_verbose;

private:
    std::string m_filename;
    std::ofstream m_ostrm;
    int m_width;
    int m_height;

    // Pour éviter les ouverture multiples
    static std::set<std::string> s_openfiles;
};

#endif // SVGFILE_H_INCLUDED



