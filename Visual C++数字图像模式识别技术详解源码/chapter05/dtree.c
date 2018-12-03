//决策树算法
enum UINT { INACTIVE, OFF, ON };
#define LN_2  0.693147180559945309417
#define entropy(x) (x > 0 ? x * log(x) / LN_2 : 0.0)

typedef struct node {
   unsigned int idx;          
   double threshold;    
   struct node *on;    
   struct node *off;    
   struct node *parent;  
} NODE;

typedef struct ne_struct {
    double ne;
    UINT status;
} NEGENTROPY;

typedef struct matrix {
   unsigned int width;
   unsigned int height;
   double **data;
} MATRIX;

MATRIX *build_matrix (unsigned int width, unsigned int height)
{
    MATRIX *_matrix;
    unsigned int i;

    _matrix = (MATRIX*) malloc (sizeof (MATRIX));
    if (!_matrix)
        err_exit (__FILE__, __LINE__);

    _matrix->width  = width;
    _matrix->height = height;

    _matrix->data = (double **) malloc (height * sizeof (double *));
    if (_matrix->data == NULL)
        err_exit(__FILE__, __LINE__);

    for (i=0; i<height; i++)
    {
        _matrix->data[i] = (double *) malloc (width * sizeof(double));
        if (_matrix->data[i] == NULL)
            err_exit(__FILE__, __LINE__);
    }
    return _matrix;
}

void err_exit (char * file, unsigned int line)
{
    printf("\n Fatal error in file %s, line %u", file, line);
    exit(0);
}

void file_size (char *file_name, unsigned int *width, unsigned int *height)
{
    FILE *f;
    unsigned int buf_size = 0xFF, _width = 0;
    char *buffer, *ptr;

    *width = *height = 0;

    buffer = (char *) malloc (buf_size * sizeof (char));
    if (buffer == NULL)
        err_exit (__FILE__, __LINE__);

    f = fopen(file_name, "r");
    if (f == NULL)
    {
        printf("\n File not found : %s\n", file_name);
        err_exit (__FILE__, __LINE__);
    }

    if (fgets(buffer, buf_size, f) != NULL)
    {
        ++*height;
        ptr = strtok (buffer, " ,");
        while (ptr != NULL)
        {
            ++*width;
            ptr = strtok (NULL, " ,");
        }
    }

    while (!feof(f))
    {
        if (fgets(buffer, buf_size, f) != NULL)
        {
            if (strlen(buffer) > strlen("\n"))  /* if line is more than a NL char */
            {
                ++*height;
                _width = 0;
                ptr = strtok (buffer, " ,");
                while (ptr != NULL)
                {
                    ++_width;
                    ptr = strtok (NULL, " ,");
                }

                if (*width != _width)
                {
                    printf("\n Number of entries in file %s did not agree", file_name);
                    err_exit (__FILE__, __LINE__);
                }
            }
        }
    }
    free (buffer);
}

void free_matrix (MATRIX *_matrix)
{
    unsigned int i;
    for (i=0; i<_matrix->height; i++)
        free (_matrix->data[i]);
    free (_matrix->data);
    free (_matrix);
}

void free_tags (char ** varname, unsigned int width)
{
    unsigned int i;
    for (i=0; i<width; i++)
        free(varname[i]);
    free (varname);
}

void free_tree ( NODE  *node )
{
    if (node == NULL)
        return;
    else
    {
        free_tree (node->on);
        free_tree (node->off);
        free(node);
    }
}

NEGENTROPY negentropy (double **data, unsigned int n_samples, NODE *local, 
unsigned int target)
{
    NEGENTROPY ret_val;
    NODE *_node, *_parent;
    UINT on_ctr, off_ctr, p1, p2, i, _match;
    double p_on, p_off, negentropy_on, negentropy_off;

    on_ctr = off_ctr = p1 = p2 = 0;

    for (i=0; i<n_samples; i++) 
    {
        _match = 1;
        _node = local;
        while (_node->parent != NULL) 
        {
            _parent = _node->parent;
            if (_node == _parent->on) 
            {
                if (data[i][_parent->idx] < _parent->threshold)
                    _match = 0;
            }
            else
                if (_node == _parent->off) 
                {
                    if (data[i][_parent->idx] >= _parent->threshold)
                        _match = 0;
                }
            _node = _parent;
        }

        if (_match) 
        {
            if (data[i][local->idx] >= local->threshold) 
            {
                on_ctr++;
                if (data[i][target] >= 0.5)
                    p1++;
            }
            else 
            {
                off_ctr++;
                if (data[i][target] >= 0.5)
                    p2++;
            }
        }
    }   

    if (on_ctr > 0)
    {
        p_on  = (REAL) p1 / (REAL) on_ctr;
        p_off = 1 - p_on;
        negentropy_on = -entropy (p_on) - entropy (p_off);
    }
    else
        negentropy_on = 0.0;

    if (off_ctr > 0)
    {
        p_on  = (REAL) p2 / (REAL) off_ctr;
        p_off = 1 - p_on;
        negentropy_off = -entropy (p_on) - entropy (p_off);
    }
    else
        negentropy_off = 0.0;

    ret_val.ne = (negentropy_on * on_ctr + negentropy_off * off_ctr);
    ret_val.ne /= (on_ctr + off_ctr);

    if ((p1 == on_ctr) && (p2 == off_ctr))
        ret_val.status = ON;
    else if ((p1 == 0) && (p2 == 0))
        ret_val.status = OFF;
    else
        ret_val.status = INACTIVE;

    return ret_val;
}

NODE* ID3 ( MATRIX *matrix, NODE* parent, unsigned int target, UINT state)
//基于Quinlan 的 ID3 算法,建立一个决策树
{
    NEGENTROPY negentropy_struct;
    NODE *node;
    unsigned int n_vars = matrix->width, n_samples = matrix->height, i, j, split;
    double **data = matrix->data;
    double best_threshold, min_negentropy, _negentropy;

    //为节点分配内存
    node = (NODE*) malloc (sizeof(NODE));
    if (!node)
        err_exit (__FILE__, __LINE__);

    //建立决策树的连接
    node->parent = parent; 
    if (parent != NULL)
    {
        if (state == ON)
            parent->on = node;
        else
            if (state == OFF)
                parent->off = node;
    }
    min_negentropy = 1.0;

    for (i=0; i<n_vars; i++)
    {
        for (j=0; j<n_samples; j++) 
        {
            if (i != target) 
            {
                node->idx = i;
                node->threshold = data[j][i];

                negentropy_struct = negentropy (data, n_samples, node, target);

                _negentropy = negentropy_struct.ne;

                if (_negentropy < min_negentropy) 
                {
                    min_negentropy = _negentropy;
                    split = i;
                    best_threshold = data[j][i];
                }
            }
        }
    }

    //保存最高信息增益的属性
    node->idx = split;
    node->threshold = best_threshold;

    if  (negentropy_struct.status != INACTIVE) 
    {
        node->on = node->off = NULL;
        node->idx = negentropy_struct.status;
    }
    else
    {
        node->on  = ID3 (matrix, node, target, ON);
        node->off = ID3 (matrix, node, target, OFF);
    }

    return node;
}
