#include <immintrin.h>
#include <cstdint>

static unsigned int g_seed;

/*
int f_memcmp(const void *p1, const void *p2, size_t n)
{
    size_t i;
    if (p1 == p2)
    {
        return 0;
    }
    for (i = 0; (i < n) && (*(uint8_t *)p1 == *(uint8_t *)p2);
        i++, p1 = 1 + (uint8_t *)p1, p2 = 1 + (uint8_t *)p2);
    return (i == n) ? 0 : (*(uint8_t *)p1 - *(uint8_t *)p2);
}*/

inline void fast_srand(int seed)
{
   g_seed = seed;
}

inline int fastrand()
{
   g_seed = (214013 * g_seed + 2531011);
   return (g_seed >> 16) & 0x7FFF;
}

/*
void fastMemcpy( void *pvDest, void *pvSrc, size_t nBytes ) {
   /*
   assert( nBytes % 32 == 0 );
   assert( ( intptr_t( pvDest ) & 31 ) == 0 );
   assert( ( intptr_t( pvSrc ) & 31 ) == 0 );*//*
   const __m256i *pSrc = reinterpret_cast<const __m256i*>( pvSrc );
   __m256i *pDest = reinterpret_cast<__m256i*>( pvDest );
   int64_t nVects = nBytes / sizeof( *pSrc );
   for( ; nVects > 0; nVects--, pSrc++, pDest++ ) {
      const __m256i loaded = _mm256_stream_load_si256( pSrc );
      _mm256_stream_si256( pDest, loaded );
   }
   _mm_sfence();
}*/
/*
void X_aligned_memcpy_sse2( void* dest, const void* src, const unsigned long size_t )
{
   __asm
   {
      mov esi, src;    //src pointer
      mov edi, dest;   //dest pointer

      mov ebx, size_t; //ebx is our counter 
      shr ebx, 7;      //divide by 128 (8 * 128bit registers)


   loop_copy:
      prefetchnta 128[ESI]; //SSE2 prefetch
      prefetchnta 160[ESI];
      prefetchnta 192[ESI];
      prefetchnta 224[ESI];

      movdqa xmm0, 0[ESI]; //move data from src to registers
      movdqa xmm1, 16[ESI];
      movdqa xmm2, 32[ESI];
      movdqa xmm3, 48[ESI];
      movdqa xmm4, 64[ESI];
      movdqa xmm5, 80[ESI];
      movdqa xmm6, 96[ESI];
      movdqa xmm7, 112[ESI];

      movntdq 0[EDI], xmm0; //move data from registers to dest
      movntdq 16[EDI], xmm1;
      movntdq 32[EDI], xmm2;
      movntdq 48[EDI], xmm3;
      movntdq 64[EDI], xmm4;
      movntdq 80[EDI], xmm5;
      movntdq 96[EDI], xmm6;
      movntdq 112[EDI], xmm7;

      add esi, 128;
      add edi, 128;
      dec ebx;

      jnz loop_copy; //loop please
   loop_copy_end:
   }
}*/