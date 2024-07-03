# myapp/views.py

from rest_framework import generics
from .models import Post
from .serializers import PostSerializer
from .services import create_post, update_post

class PostListCreateView(generics.ListCreateAPIView):
    queryset = Post.objects.all()
    serializer_class = PostSerializer

    def perform_create(self, serializer):
        create_post(serializer.validated_data['title'], serializer.validated_data['content'])

class PostDetailView(generics.RetrieveUpdateDestroyAPIView):
    queryset = Post.objects.all()
    serializer_class = PostSerializer

    def perform_update(self, serializer):
        update_post(self.get_object().id, serializer.validated_data['title'], serializer.validated_data['content'])