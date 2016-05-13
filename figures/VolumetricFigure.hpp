#ifndef RT_VOLUMETRIC_FIGURE
#define RT_VOLUMETRIC_FIGURE

#include "Figure.hpp"
#include "../geometry/BasicGeom.hpp"
#include <cstdio>
#include <algorithm>

using std::swap;
using std::make_pair;
using std::vector;

using namespace BasicGeom;

class VolumetricFigure : public Figure {
public:
    virtual vector<myFloat> intersectionTimes(const Ray &) const = 0;
    virtual bool inside(const Vector &) const = 0;

    virtual ~VolumetricFigure() {

    }
};

class IntersectionFigure: public VolumetricFigure {
    VolumetricFigure *figures[2];
    pair<int, myFloat> innerIntersection(const Ray &r) const {
        vector<myFloat> inter[2] = {figures[0]->intersectionTimes(r),
                                    figures[1]->intersectionTimes(r)};
        myFloat ans = 1e18;
        int figure = -1;
        for (int z = 0; z < 2; ++z) {
            for (int i = 0; i < inter[z].size(); ++i) {
                if (figures[1 - z]->inside(r.start + r.direction
                                                     * inter[z][i])) {
                    if (less(inter[z][i], ans)) {
                        ans = inter[z][i];
                        figure = z;
                    }
                    break;
                }
            }
        }
        return make_pair(figure, ans);
    }
public:
    IntersectionFigure(VolumetricFigure *first, VolumetricFigure *second) {
        figures[0] = first;
        figures[1] = second;
    }

    virtual vector<myFloat> intersectionTimes(const Ray &r) const {
        vector<myFloat> inter[2] = {figures[0]->intersectionTimes(r),
                                    figures[1]->intersectionTimes(r)};
        for (int z = 0; z < 2; ++z) {
            for (int i = 0; i < inter[z].size(); ++i) {
                if (!figures[1 - z]->inside(r.start + r.direction
                                                      * inter[z][i])) {
                    swap(inter[z][i], inter[z].back());
                    inter[z].pop_back();
                }
            }
        }
        vector <myFloat> result(inter[0].size() + inter[1].size());
        merge(inter[0].begin(), inter[0].end(),
              inter[1].begin(), inter[1].end(),
              result.begin());
        return result;
    }

    virtual Vector rayIntersection(const Ray &r) const {
        auto inner = innerIntersection(r);
        if (inner.first == -1) {
            return NONE;
        }
        return r.start + r.direction * inner.second;
    }

    virtual Plane getTangentPlane(const Vector &r) const {
        if (figures[0]->on(r) && figures[1]->inside(r)) {
            return figures[0]->getTangentPlane(r);
        }
        return figures[1]->getTangentPlane(r);
    }

    virtual BoundingBox getBoundingBox() const {
        return (figures[0]->getBoundingBox() |  figures[1]->getBoundingBox());
    }

    virtual string name() const {
        return "Volumetric Figure " + figures[0]->name() + " "
               + figures[1]->name();
    }

    virtual bool on(const Vector &v) const {
        return (figures[0]->on(v) && figures[1]->inside(v))
               || (figures[1]->inside(v) && figures[0]->on(v));
    }

    bool inside(const Vector &r) const {
        return figures[0]->inside(r) && figures[1]->inside(r);
    }

    ~IntersectionFigure() {
        delete figures[0];
        delete figures[1];
    }
};


#endif
