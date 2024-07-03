# myapp/services.py

from .models import Post

def get_all_posts():
    return Post.objects.all()

def get_post_by_id(post_id):
    return Post.objects.get(id=post_id)

def create_post(title, content):
    post = Post(title=title, content=content)
    post.save()
    return post

def update_post(post_id, title, content):
    post = Post.objects.get(id=post_id)
    post.title = title
    post.content = content
    post.save()
    return post

def delete_post(post_id):
    post = Post.objects.get(id=post_id)
    post.delete()