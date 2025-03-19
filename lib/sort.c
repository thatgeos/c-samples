void sort (int a[], int n)
{
    if (n == 1)
    {
        return;
    }
    else
    {
        int min = 9999;
        int p = 9999;
        for (int i = 0; i < n; i+= 1)
        {
            if (a[i] < min)
            {
                min = a[i];
                p = i;
            }
        }

        for (int i = p; i > 0; i -= 1)
        {
            a[i] = a[i-1];
        }
        a[0] = min;
    }
    int b[n-1];

    for (int i = 0; i < (n-1); i += 1)
    {
        b[i] = a[i+1];
    }
    sort(b, n-1);
    for (int i = (n-1); i > 0; i -= 1)
    {
        a[i] = b[i-1];
    }   
}