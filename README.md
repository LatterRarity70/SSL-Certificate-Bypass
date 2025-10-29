## useless due to lack of proper game research: game already ignores ssl stuff... 
```cpp
// from source of geometry dash network lib:
curl_easy_setopt(v10, 0x40i64, 0i64); // CURLOPT_SSL_VERIFYPEER = 0
curl_easy_setopt(v10, 0x51i64, 0i64); // CURLOPT_SSL_VERIFYHOST = 0  
curl_easy_setopt(v10, 0x63i64, 1i64); // CURLOPT_SSL_ENABLE_NPN = 1
```
### consider this project as mistake.

---

# SSL Certificate Bypass

bypasses ssl certificate verification in gd

## why

Roskomnadzor (RKN) blocked ech (encrypted client hello) so newgrounds/gd cdn don't work from russia anymore. this mod disables ssl verification to allow loading music and working with online content

## what it does

- hooks `curl_easy_init`
- disables `CURLOPT_SSL_VERIFYPEER` and `CURLOPT_SSL_VERIFYHOST`
- that's it

<co>
 
## security

yes, this makes connections vulnerable to </c><cr>mitm attacks</c><co>. but for a game it's not critical, and the alternative is having no access to content at all

use at your own risk

</c>

## platforms

- windows
- android (<cr>experimental</c>)
