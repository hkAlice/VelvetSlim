// About 5~8% faster.

void Velvet::Renderer::fastDrawLine( Vec2Int p0, Vec2Int p1, Pixel color )
{
   int		x, y;
   int		dx, dy;
   int		incx, incy;
   int		balance;

   if( p1.x >= p0.x )
   {
      dx = p1.x - p0.x;
      incx = 1;
   }
   else
   {
      dx = p0.x - p1.x;
      incx = -1;
   }

   if( p1.y >= p0.y )
   {
      dy = p1.y - p0.y;
      incy = 1;
   }
   else
   {
      dy = p0.y - p1.y;
      incy = -1;
   }

   x = p0.x;
   y = p0.y;

   if( dx >= dy )
   {
      dy <<= 1;
      balance = dy - dx;
      dx <<= 1;

      while( x != p1.x )
      {
         drawPixel( x, y, color );
         if( balance >= 0 )
         {
            y += incy;
            balance -= dx;
         }
         balance += dy;
         x += incx;
      } drawPixel( x, y, color );
   }
   else
   {
      dx <<= 1;
      balance = dx - dy;
      dy <<= 1;

      while( y != p1.y )
      {
         drawPixel( x, y, color );
         if( balance >= 0 )
         {
            x += incx;
            balance -= dy;
         }
         balance += dx;
         y += incy;
      } drawPixel( x, y, color );
   }
}