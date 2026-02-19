#version 460 core
layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer ChunkA
{
    float dataA[];
};


layout(std430, binding = 1) buffer ChunkB
{
    float dataB[];
};


uniform int u_borderLength;
uniform int u_edgeType;
uniform int u_lineVerts;

int normalOffset(int col, int row)
{
    int vertexIndex = row * u_lineVerts + col;
    return vertexIndex * 8 + 3;   
}

void main()
{
    int tid = int(gl_GlobalInvocationID.x);
    if (tid >= u_borderLength)
        return;
    int colA, rowA;
    int colB, rowB;
    int last = u_lineVerts - 1;

    switch (u_edgeType)
    {
        case 0: // A's right col - B's left col
            colA = last;  rowA = tid;
            colB = 0;     rowB = tid;
            break;
        case 1: // A's left col - B's right col
            colA = 0;     rowA = tid;
            colB = last;  rowB = tid;
            break;
        case 2: // A's top row - B's bottom row
            colA = tid;   rowA = last;
            colB = tid;   rowB = 0;
            break;
        case 3: // A's bottom row - B's top row
            colA = tid;   rowA = 0;
            colB = tid;   rowB = last;
            break;
        default:
            return;
    }

    int iA = normalOffset(colA, rowA);
    int iB = normalOffset(colB, rowB);
    vec3 nA = vec3(dataA[iA], dataA[iA + 1], dataA[iA + 2]);
    vec3 nB = vec3(dataB[iB], dataB[iB + 1], dataB[iB + 2]);
    vec3 smoothed = normalize(nA + nB);

    //writing back to buffers
    dataA[iA]     = smoothed.x;
    dataA[iA + 1] = smoothed.y;
    dataA[iA+ 2] = smoothed.z;

    dataB[iB]     = smoothed.x;
    dataB[iB+1] = smoothed.y;
    dataB[iB+2] = smoothed.z;
}
