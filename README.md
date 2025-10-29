## cancelled and useless due to lack of proper verification and research of the game, consider this project a mistake.

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
