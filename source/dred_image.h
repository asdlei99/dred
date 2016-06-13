
typedef struct
{
    float scale;
    unsigned int width;
    unsigned int height;
    drgui_image_format format;
    void* pImageData;
} dred_image_desc;

typedef struct
{
    unsigned int referenceCount;
    float scale;
    drgui_image* pGUIImage;
} dred_subimage;

struct dred_image
{
    dred_context* pDred;
    unsigned int id;
    dred_image_desc desc;
    size_t subimageCount;
    dred_subimage* pSubImages;

    dred_image_library* pLibrary;   // Can be null, in which case the image is not managed by a library.
    unsigned int referenceCount;    // Used by the image library that owns the font.
    
};

dred_image* dred_image_create(dred_context* pDred, unsigned int id, dred_image_desc* pDesc, size_t descCount);
void dred_image_delete(dred_image* pImage);

drgui_image* dred_image_acquire_subimage(dred_image* pImage, float scale);
void dred_image_release_subimage(dred_image* pImage, drgui_image* pSubImage);
