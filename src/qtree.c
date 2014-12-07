#include "qtree.h"

static bool _qtree_find(QTree_t * qtree, Point_t point, QTree_t ** result);

QTree_t qtree_init(Box_t boundary)
{
    QTree_t qtree;

    qtree.boundary = boundary;
    qtree.tagged_data = malloc(sizeof(TaggedData_t));
    *qtree.tagged_data = (TaggedData_t) {
        (Point_t) {boundary.nw.x, boundary.nw.y}, NULL
    };
    qtree.vacant = true;
    qtree.nw = NULL;
    qtree.ne = NULL;
    qtree.sw = NULL;
    qtree.se = NULL;
    return qtree;
}

void qtree_destroy(QTree_t * qtree)
{
    if (qtree->tagged_data != NULL) {
        free(qtree->tagged_data);
    }
    if (qtree->nw != NULL) {
        qtree_destroy(qtree->nw);
        free(qtree->nw);
    }
    if (qtree->ne != NULL) {
        qtree_destroy(qtree->ne);
        free(qtree->ne);
    }
    if (qtree->sw != NULL) {
        qtree_destroy(qtree->sw);
        free(qtree->sw);
    }
    if (qtree->se != NULL) {
        qtree_destroy(qtree->se);
        free(qtree->se);
    }
}

void qtree_display(QTree_t qtree)
{
    printf("QTree ");
    box_display(qtree.boundary);
    if (!qtree.vacant) {
        printf(" has data: %p", qtree.tagged_data->data);
        printf(" at point ");
        point_display(qtree.tagged_data->point);
    }
    if (qtree.nw != NULL) {
        printf(" - parent of ...\n");
    }
    if (qtree.nw != NULL) {
        printf("\t");
        qtree_display(*qtree.nw);
    }
    if (qtree.ne != NULL) {
        printf("\t");
        qtree_display(*qtree.ne);
    }
    if (qtree.sw != NULL) {
        printf("\t");
        qtree_display(*qtree.sw);
    }
    if (qtree.se != NULL) {
        printf("\t");
        qtree_display(*qtree.se);
    }
    printf("\n");
}

bool qtree_subdivide(QTree_t * qtree)
{
    int top_y, middle_y, bottom_y;
    int left_x, middle_x, right_x;
    int width, height;

    // Auxillary variables for boundaries of current node
    width = qtree->boundary.width;
    height = qtree->boundary.height;
    if (width == 0 && height == 0) {
        return false;
    }
    top_y = box_get_n(qtree->boundary);
    bottom_y = box_get_s(qtree->boundary);
    middle_y = (top_y + bottom_y) / 2 + 1;
    left_x = box_get_w(qtree->boundary);
    right_x = box_get_e(qtree->boundary);
    middle_x = (left_x + right_x) / 2 + 1;

    // Construct boundaries of child nodes
    Box_t nw_boundary = {(Point_t) {left_x, top_y}, width / 2, height / 2};
    Box_t ne_boundary = {(Point_t) {middle_x, top_y}, width / 2, height / 2};
    Box_t sw_boundary = {(Point_t) {left_x, middle_y}, width / 2, height / 2};
    Box_t se_boundary = {(Point_t) {middle_x, middle_y}, width / 2, height / 2};
    // For even parent trees, reduce lower and/or right children
    if (width % 2 == 0) {
        ne_boundary.width -= 1;
        se_boundary.width -= 1;
    }
    if (height % 2 == 0) {
        sw_boundary.height -= 1;
        se_boundary.height -= 1;
    }
    // Create child node qtrees
    qtree->nw = malloc(sizeof(QTree_t));
    qtree->ne = malloc(sizeof(QTree_t));
    qtree->sw = malloc(sizeof(QTree_t));
    qtree->se = malloc(sizeof(QTree_t));
    *qtree->nw = qtree_init(nw_boundary);
    *qtree->ne = qtree_init(ne_boundary);
    *qtree->sw = qtree_init(sw_boundary);
    *qtree->se = qtree_init(se_boundary);
    return true;
}

bool qtree_insert(QTree_t * qtree, Point_t point, void * data)
{
    // Point cannot fit in qtree
    if (!box_has_point(qtree->boundary, point)) {
        return false;
    }
    // If there is space in this quad tree, add the object here
    if (qtree->vacant) {
        *qtree->tagged_data = (TaggedData_t) {point, data};
        qtree->vacant = false;
        return true;
    }
    // subdivide if needed and the node isn't only one cell
    if (qtree_is_leaf(qtree)) {
        if (!qtree_subdivide(qtree)) {
            return false;
        }
    }
    // Try to insert in any of the four children
    if (qtree_insert(qtree->nw, point, data)) {
        return true;
    }
    if (qtree_insert(qtree->ne, point, data)) {
        return true;
    }
    if (qtree_insert(qtree->sw, point, data)) {
        return true;
    }
    if (qtree_insert(qtree->se, point, data)) {
        return true;
    }
    // Could not be added. (on same point as another)
    return false;
}

void list_qtree_query_range(QTree_t * qtree, Box_t range, LList_t * list)
{
    Point_t point;

    // Automatically abort if the range does not intersect this qtree
    if (!box_intersects_box(range, qtree->boundary)) {
        return;
    }
    // Add qtree's data if it has any in range
    if (!qtree->vacant) {
        point = qtree->tagged_data->point;
        if (box_has_point(range, point)) {
            list_append(list, &qtree->tagged_data);
        }
    }
    // Terminate here if there are no children
    if (qtree_is_leaf(qtree)) {
        return;
    }
    // Add the points from the children
    list_qtree_query_range(qtree->nw, range, list);
    list_qtree_query_range(qtree->ne, range, list);
    list_qtree_query_range(qtree->sw, range, list);
    list_qtree_query_range(qtree->se, range, list);
}


bool _qtree_find(QTree_t *qtree, Point_t point, QTree_t **result_tree)
{
    Point_t current_point;

    // Exit if point can't be in this node
    if (!box_has_point(qtree->boundary, point)) {
        return false;
    }
    // Add qtree's data if it has any in range
    if (!qtree->vacant) {
        current_point = qtree->tagged_data->point;
        if (point.x == current_point.x && point.y == current_point.y) {
            *result_tree = qtree;
            return true;
        }
    }
    // Terminate here if there are no children
    if (qtree_is_leaf(qtree)) {
        return false;
    }
    // Search the points in the children
    if (_qtree_find(qtree->nw, point, result_tree)) {
        return true;
    }
    if (_qtree_find(qtree->ne, point, result_tree)) {
        return true;
    }
    if (_qtree_find(qtree->sw, point, result_tree)) {
        return true;
    }
    if (_qtree_find(qtree->se, point, result_tree)) {
        return true;
    }
    return false;
}

bool qtree_peek(QTree_t * qtree, Point_t point, void ** result)
{
    QTree_t * result_qtree;

    if (_qtree_find(qtree, point, &result_qtree)) {
        *result = result_qtree->tagged_data->data;
        return true;
    }
    return false;
}

bool qtree_pop(QTree_t * qtree, Point_t point, void ** result)
{
    QTree_t * result_qtree;

    if (_qtree_find(qtree, point, &result_qtree)) {
        *result = result_qtree->tagged_data->data;
        result_qtree->vacant = true;
        result_qtree->tagged_data->point = result_qtree->boundary.nw;
        result_qtree->tagged_data->data = NULL;
        return true;
    }
    return false;
}
